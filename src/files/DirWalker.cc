

#include "DirWalker.h"
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include "utils/StreamBuffer.h"
#include "utils/SlashDef.h"

#if !defined(_MSC_VER)

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#else

#include <windows.h>

#endif

#include "deps/xxhash/xxhash.h"

#define _DirFontStart	"<font color=\"blue\"><b>"
#define _DirFontEnd		"</b></font>"
#define _PREDESTINE		"fetch"

bool getFileHash(const char *fullpath, unsigned int seed, unsigned int &hashResult);


// Though seed can be zero
// But we take zero seed as no hash
struct FileItem {
public:
	FileItem(const char *name, unsigned int hashValue)
		:_name(name), _hasHash(true)
		,_hashValue(hashValue) {}

	FileItem(const char *name)
		:_name(name), _hasHash(false)
		,_hashValue(0) {}

public:
	std::string toStr(const char *rootpath) const {
		return _hasHash ? toStrHash(rootpath): toStrNoHash(rootpath);
	}

	bool operator<(const FileItem&rhs)const{
		return _name < rhs._name;
	}

private:
	std::string stripHash(const char *rootpath, std::string &name)const{
		int lz = strlen(rootpath);
		if (!strncmp(_name.data(), rootpath, lz) ) {
			name = _name.substr(lz);
		} else {
			name = _name;
		}
		char href[BUFSIZ];
		snprintf(href, sizeof(href), "\"%s/%s\"", _PREDESTINE, name.c_str());
		for(auto &ch:href){
			if ('\\'==ch){
				ch = '/';
			}
		}
		return href;
	}

	std::string toStrNoHash(const char *rootpath)const{
		std::string name;
		std::string href = stripHash(rootpath, name);
		char buf[BUFSIZ + 1024];
		snprintf(buf, sizeof(buf), "<a href=%s>%s</a><br/>\n", href.c_str(), name.c_str());
		return buf;
	}

	std::string toStrHash(const char *rootpath)const {
		std::string name;
		std::string href = stripHash(rootpath, name);
		char buf[BUFSIZ+1024];
		snprintf(buf, sizeof(buf), "<a href=%s xxhash=\"%08x\">%s</a><br/>\n"
			, href.c_str()
			, _hashValue
			, name.c_str());
		return buf;
	}

private:
	std::string _name;
	unsigned int _hashValue;
	bool _hasHash;
};

typedef std::vector<FileItem> FVS;
typedef std::vector<std::string> VS;

int filterByName(const char *name)
{
	if(!strcmp(name, ".")  ||
	   !strcmp(name, "..") ||
	   !strcmp(name, ".git")) {
		return 1; //bingo
	}
	return 0;
}

void formatContentByInfo(FVS &files, VS &dirs, const char *root, std::string &content)
{
	std::sort(files.begin(), files.end());
	std::sort(dirs.begin(), dirs.end());

	char rootpath[BUFSIZ + BUFSIZ + 256];
	strcpy(rootpath, root);
	int lz = strlen(rootpath);
	if(lz>0 && rootpath[lz-1] != '/' && rootpath[lz-1] != '\\')	{
		strcat(rootpath, _END_SLASH_STR);
		++lz;
	}

	StreamBuffer b;
	std::for_each(files.begin(), files.end(), [&](const FileItem &f){
		std::string str = f.toStr(rootpath);
		b.append(str);
	});

	for(auto &d:dirs) {
		char buf[BUFSIZ+1000];
		std::string name;
		if ( !strncmp(d.data(), rootpath, lz) )	{
			name = d.substr(lz);
		} else {
			name = d;
		}
		snprintf(buf, sizeof(buf), "%s&lt;%s&gt;<br/>%s\n", _DirFontStart, name.c_str(), _DirFontEnd);
		b.append(buf, strlen(buf));
	}
	b.readString(content);
	//printf("The response length is %ld\n", content.size());
}

#if defined(_MSC_VER)
//Windows implementation.
//For debugging only.
// Refer:
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb540534(v=vs.85).aspx
bool getFileHash(const char *fullpath, unsigned int seed, unsigned int &hashResult) {
	HANDLE hFile = CreateFileA(fullpath,
		GENERIC_READ,
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // | FILE_FLAG_OVERLAPPED, // normal file
		NULL
	);         
	if(INVALID_HANDLE_VALUE==hFile){
		return false;
	}
	const int lsize = 1024*1024*1;
	char *buffer = (char*)::HeapAlloc(::GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, lsize);
	void *state = XXH32_init(seed);
	DWORD dwBytesRead = 0;
	while (::ReadFile(hFile, buffer, lsize, &dwBytesRead, NULL)) {
		if (dwBytesRead == 0)
			break;
		XXH32_update(state, buffer, dwBytesRead);
	}
	::CloseHandle(hFile);
	::HeapFree(::GetProcessHeap(), 0, buffer);
	hashResult = XXH32_digest(state);
	return true; // Result is validated.
}

void elicitDirSub(const char *now
				  , FVS& fv
				  , VS&  dv
				  , bool doHash
				  , unsigned int seed) {
	char curpath[MAX_PATH+1];
	strcpy(curpath, now);
	int lz = strlen(curpath);
	if(lz>0 && curpath[lz-1] != '/' && curpath[lz-1] != '\\')
	{
		strcat_s(curpath, "\\");
	}
	char searchText[MAX_PATH+1];
	strcpy(searchText, curpath);
	strcat_s(searchText, "*.*");
	WIN32_FIND_DATAA findFile;
	HANDLE hFind = ::FindFirstFileA(searchText, &findFile);
	if(INVALID_HANDLE_VALUE==hFind)
	{
		return;
	}

	std::vector<std::string> nd;
	do
	{
		char fullpath[MAX_PATH + 100];
		strcpy_s(fullpath, curpath); // ends with slash
		strcat_s(fullpath, findFile.cFileName);
		if( findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			if( filterByName(findFile.cFileName) )
			{
				continue;
			}
			nd.push_back(fullpath);
		} 
		else
		{
			unsigned int hashv = 0;
			if(doHash && getFileHash(fullpath, seed, hashv)) {
				fv.push_back(FileItem(fullpath, hashv));
			} else {
				fv.push_back(FileItem(fullpath));
			}
		}
	}while(::FindNextFileA(hFind, &findFile));
	::FindClose(hFind);
	hFind = INVALID_HANDLE_VALUE;
	for(const auto &d:nd)
	{
		elicitDirSub(d.c_str(), fv, dv, doHash, seed);
	}
	std::copy(nd.begin(), nd.end(), std::back_inserter(dv));
}

#else  // Linux/Unix

bool getFileHash(const char *fullpath, unsigned int seed, unsigned int &hashResult) {
	int fin = open(fullpath, O_RDONLY, S_IRWXU);
	if (fin<0) {
		return false;
	}
	int lsize = 1024 * 256;
	char buf[lsize];
	ssize_t nRead = 0;
	void *state = XXH32_init(seed);
	while( (nRead = read(fin, buf, lsize)) > 0) {
		XXH32_update(state, buf, nRead);
	}
	close(fin);
	hashResult = XXH32_digest(state);
	return true;
}

void elicitDirSub(const char *now, FVS &fv, VS &dv, bool doHash, unsigned int seed)
{
	char path[1000 + BUFSIZ];
	strcpy(path, now);
	if ( strlen(path)>0 && path[strlen(path)-1]!='/' )
	{
		//printf("patched!\n");
		strcat(path, "/");
	}
	DIR* dp = opendir(path);
	if (!dp)
	{
		// std::ostringstream os;
		// os<<"error opening <"<<now<<">";
		// content = os.str();
		return;
	}

	struct stat theS;
	struct dirent *entry;
	std::vector<std::string> dirs;
	while (entry = readdir(dp))
	{
		if(filterByName(entry->d_name))
		{
			continue;
		}
		char newp[1000 + BUFSIZ];
		snprintf(newp, sizeof(newp), "%s%s", path, entry->d_name);
		if (-1 == stat(newp, &theS)) 
		{
			// printf("Error stat for <%s>\n", newp);
			// perror("stat");
			continue;
		}
		if (S_ISDIR(theS.st_mode)) // if directory, then add a "/" to current path
		{
			dirs.push_back(newp);
		}
		else
		{
			unsigned int hashv = 0;
			if(doHash && getFileHash(newp, seed, hashv)){
				fv.push_back(FileItem(newp, hashv));
			} else {
				fv.push_back(FileItem(newp));
			}
		}
	}
	closedir(dp);
	for(const auto &d:dirs)	{
		//printf(">> \"%s\"\n", d.c_str());
		elicitDirSub(d.c_str(), fv, dv, doHash, seed);
	}
	std::copy(dirs.begin(), dirs.end(), std::back_inserter(dv));
}

#endif

void elicitDir(const char *root, std::string &content, bool doHash, unsigned int seed)
{
	//content = "Windows(Not implemented yet)";
	FVS files;
	VS dirs;
	elicitDirSub(root, files, dirs, doHash, seed);
	formatContentByInfo(files, dirs, root, content);
}


