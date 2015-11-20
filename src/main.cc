

#include "Inception.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>

int main() {
    srand((unsigned int)time(0));
#ifdef _DIRECTORY_DIST
    printf("Directory server starting ... \n");
#else
    printf("World server starting ... \n");
#endif

	inception::masterLoop();
}


