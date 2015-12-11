


#include "Inception.h"
#include "StreamBuffer.h"

//For some test.
#include "test/TestDep.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

char* retrieveCurrentDir(char *buffer, size_t length)
{
#ifdef _WIN32
	GetCurrentDirectoryA(length, buffer);
	return buffer;
#else
	return getcwd(buffer, length);
#endif
}

int main()
{
	int rv = 0;
	char path[BUFSIZ*2];
	if(char *p = retrieveCurrentDir(path, sizeof(path))){
		rv = inception::httpStaticFileLoop(p);
	}
	return rv;
}