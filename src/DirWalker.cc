

#include "DirWalker.h"
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include "StreamBuffer.h"

#if !defined(_MSC_VER)

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#else

#include <windows.h>

#endif

#if defined(_MSC_VER)
#	define snprintf sprintf_s
#	define _END_SLASH			'\\'
#	define _END_SLASH_STR		"\\"
#else
#	define _END_SLASH			'/'
#	define _END_SLASH_STR		"/"
#endif

#define _DirFontStart	"<font color=\"blue\"><b>"
#define _DirFontEnd		"</b></font>"
#define _PREDESTINE		"fetch"

int filterByName(const char *name)
{
	if(!strcmp(name, ".")  ||
	   !strcmp(name, "..") ||
	   !strcmp(name, ".git")) {
		return 1; //bingo
	}
	return 0;
}

void formatContentByInfo(std::vector<std::string> &files, std::vector<std::string> &dirs, const char *root, std::string &content)
{
	std::sort(files.begin(), files.end());
	std::sort(dirs.begin(), dirs.end());

	char rootpath[BUFSIZ + BUFSIZ + 256];
	strcpy(rootpath, root);
	int lz = strlen(rootpath);
	if(lz>0 && rootpath[lz-1] != '/' && rootpath[lz-1] != '\\')
	{
		strcat(rootpath, _END_SLASH_STR);
		++lz;
	}

	StreamBuffer b;
	for(auto &f:files)
	{
		char buf[BUFSIZ+1000];
		std::string name;
		if (!strncmp(f.data(), rootpath, lz) )
		{
			name = f.substr(lz);
		}
		else
		{
			name = f;
		}

		char hrefBuff[BUFSIZ+1000];
		snprintf(hrefBuff, sizeof(hrefBuff), "\"%s/%s\"", _PREDESTINE, name.c_str());
		for(auto &ch:hrefBuff)
		{
			if ('\\'==ch)
			{
				ch = '/';
			}
		}
		snprintf(buf, sizeof(buf), "<a href=%s>%s</a><br/>\n", hrefBuff, name.c_str());
		b.append(buf, strlen(buf));
	}
	for(auto &d:dirs)
	{
		char buf[BUFSIZ+1000];
		std::string name;
		if ( !strncmp(d.data(), rootpath, lz) )
		{
			name = d.substr(lz);
		}
		else
		{
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

void elicitDirSub(const char *now, std::vector<std::string>& fv, std::vector<std::string> &dv)
{
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
			fv.push_back(fullpath);
		}
	}while(::FindNextFileA(hFind, &findFile));
	::FindClose(hFind);
	hFind = INVALID_HANDLE_VALUE;
	for(const auto &d:nd)
	{
		elicitDirSub(d.c_str(), fv, dv);
	}
	std::copy(nd.begin(), nd.end(), std::back_inserter(dv));
}

void elicitDir(const char *root, std::string &content)
{
	content = "Windows(Not implemented yet)";
	std::vector<std::string> files, dirs;
	elicitDirSub(root, files, dirs);
	formatContentByInfo(files, dirs, root, content);
}

#else  // Linux/Unix

void elicitDirSub(const char *now, std::vector<std::string> &fv, std::vector<std::string> &dv)
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
			fv.push_back(newp);
		}
	}
	closedir(dp);
	for(const auto &d:dirs)
	{
		//printf(">> \"%s\"\n", d.c_str());
		elicitDirSub(d.c_str(), fv, dv);
	}
	std::copy(dirs.begin(), dirs.end(), std::back_inserter(dv));
}

void elicitDir(const char *root, std::string &content)
{
	std::vector<std::string> filev, dirs;
	//printf("Starting from <%s>\n", root);

	elicitDirSub(root, filev, dirs);
	formatContentByInfo(filev, dirs, content);
}

#endif
