


#include "Inception.h"
#include "StreamBuffer.h"
#include <unistd.h>

int main()
{
	int rv = 0;
	char path[BUFSIZ*2];
	if(char *p = getcwd(path, sizeof(path))){
		rv = inception::helloLoop(p);
	}
	return rv;
}
