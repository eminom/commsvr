
#ifndef _XASSERT__DEF__SVR__
#define _XASSERT__DEF__SVR__


#define XAssert(condition, msg)\
	if(!(condition)){\
		fprintf(stderr, "Assertion failure: %s at %d\n", __FILE__, __LINE__);\
		fprintf(stderr, "Condition: %s\n", #condition);\
		fprintf(stderr,"%s\n", msg);\
		abort();\
	}

#endif