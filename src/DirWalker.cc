

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

#endif

#if defined(_MSC_VER)
#define snprintf sprintf_s
#endif

int filterByName(const char *name)
{
	if(!strcmp(name, ".") ||
	   !strcmp(name, "..") ||
	   !strcmp(name, ".git")){
		return 1; //bingo
	}
	return 0;
}

void formatContentByInfo(std::vector<std::string> &files, std::vector<std::string> &dirs, std::string &content)
{
	std::sort(files.begin(), files.end());
	std::sort(dirs.begin(), dirs.end());
	
	StreamBuffer b;
	for(auto &f:files)
	{
		char buf[BUFSIZ+1000];
		snprintf(buf, sizeof(buf), "%s<br/>\n", f.c_str());
		b.append(buf, strlen(buf));
	}
	for(auto &d:dirs)
	{
		char buf[BUFSIZ+1000];
		snprintf(buf, sizeof(buf), "&lt;%s&gt;<br/>\n", d.c_str());
		b.append(buf, strlen(buf));
	}
	b.readString(content);
	//printf("The response length is %ld\n", content.size());
}

#if defined(_MSC_VER)

void elicitDir(const char *root, std::string &content)
{
	content = "Windows(Not implemented yet)";
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
