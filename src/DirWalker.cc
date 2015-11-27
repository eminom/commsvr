


#include "DirWalker.h"
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

#if !defined(_MSC_VER)

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

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


#if defined(_MSC_VER)

void elicitDir(const char *root, std::string &content)
{
	content = "Windows(Not implemented yet)";
}

#else  // Linux/Unix

void elicitDir(const char *root, std::string &content)
{
	char path[1000 + BUFSIZ];
	strcpy(path, root);
	if ( strlen(path)>0 && path[strlen(path)-1]!='/' )
	{
		strcat(path, "/");
	}
	DIR* dp;
	if ( !(dp = opendir(path)) )
	{
		std::ostringstream os;
		os<<"error opening <"<<root<<">";
		content = os.str();
		return;
	}

	struct stat theS;
	struct dirent *entry;
	std::vector<std::string> filev;
	std::vector<std::string> dirs;
	while (entry = readdir(dp))
	{
		if(filterByName(entry->d_name))
		{
			continue;
		}
		char newp[1000 + BUFSIZ];
		snprintf(newp, sizeof(newp), "%s/%s", path, entry->d_name);
		if (-1 == stat(newp, &theS)) 
		{
			// printf("Error stat for <%s>\n", newp);
			// perror("stat");
			continue;
		}
		if (S_ISDIR(theS.st_mode)) // if directory, then add a "/" to current path
		{
			/*
			#if defined(_STD_ALONE_DIST)
				output<<"<"<<entry->d_name<<">"<<"<br/>\n";
			#else
				output<<"&lt;"<<entry->d_name<<"&gt;"<<"<br/>\n";
			#endif
			*/
			//read(path);
			//strcpy(path, pth);
			dirs.push_back(entry->d_name);
		}
		else
		{
			filev.push_back(entry->d_name);
		}
	}
	closedir(dp);
	std::sort(filev.begin(), filev.end());
	std::sort(dirs.begin(), dirs.end());
	std::ostringstream output;
	for(auto f:filev)
	{
		output<<f<<"<br/>";
	}
	for(auto d:dirs)
	{
		output<<"&lt;"<<d<<"&gt;"<<"<br/>\n";
	}
	content = output.str();
}

#endif
