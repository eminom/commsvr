
#ifdef _MSC_VER
#include <Windows.h>
#include <ShlObj.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif

#include <string>

#ifdef _MSC_VER

void MkdirRecur(const std::string& pre)
{
	char pathbuff[BUFSIZ+MAX_PATH+1] = "";
	strcpy(pathbuff, pre.c_str());
	for(int i=strlen(pathbuff)-1;i>=0;--i){
		if('/' == pathbuff[i]){
			pathbuff[i] = '\\';
		}
	}
	SECURITY_ATTRIBUTES sa = {sizeof(sa)};
	SHCreateDirectoryExA(NULL, pathbuff, &sa);
}


#else

void MkdirRecur(const std::string& target)
{
	if(!target.size())
	{
		return;
	}
	std::string rbuf = target;
	size_t len = rbuf.size();
	if (rbuf[len-1]!='/')
	{
		rbuf += "/";
	}
	const char *pv = rbuf.c_str();
	const char *nx = strchr(pv+1, '/');
	while(nx)
	{
		char buf[BUFSIZ];
		bzero(buf, sizeof(buf));
		strncpy(buf, pv, nx - pv);
		//printf("<%s>\n", buf);
		nx = strchr(nx+1, '/');
		mkdir(buf, 0777);
		if(access(buf, 0)) 
		{
			printf("Failed to create chain <%s>\n", target.c_str());
			break;
		}
	}
	//printf("<%s>\n", str);
}

#endif