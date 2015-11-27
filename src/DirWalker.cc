


#include "DirWalker.h"
#include <sstream>
#include <string>

#if !defined(_MSC_VER)

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#endif

#if defined(_MSC_VER)

void elicitDir(const char *root, std::string &content)
{
	content = "Windows(Not implemented yet)";
}

#else  // Linux/Unix

void elicitDir(const char *root, std::string &content)
{
	char path[1000];
	strcpy(path, root);
	/*structure for storing inode numbers and files in dir
	struct dirent
	{
		ino_t d_ino;
		char d_name[NAME_MAX+1]
	}
	*/
	DIR* dp;
	if ( !(dp = opendir(path)) )
	{
		//perror("dir\n");
		std::ostringstream os;
		os<<"error opening <"<<root<<">";
		content = os.str();
		return;
	}
	char newp[1000];
	struct stat buf;
	struct dirent *files;
	while (files = readdir(dp))
	{
		if ( ! strcmp(files->d_name,".") || ! strcmp(files->d_name,"..") )
		{
			continue;
		}
		strcpy(newp, path);
		strcat(newp, "/");
		strcat(newp, files->d_name); 
		//printf("%s\n",newp);

		//stat function return a structure of information about the file    
		if( -1 == stat(newp, &buf)) 
		{
			perror("stat");
			continue;
		}
		if ( S_ISDIR(buf.st_mode) ) // if directory, then add a "/" to current path
		{
			strcat(path,"/");
			strcat(path, files->d_name);
			//read(path);
			//strcpy(path, pth);
		}
	}
	closedir(dp);
}

#endif