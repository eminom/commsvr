
#ifndef _SLASH_DEF_HEADER_
#define _SLASH_DEF_HEADER_

#if defined(_MSC_VER)
#	define snprintf sprintf_s
#	define _END_SLASH			'\\'
#	define _END_SLASH_STR		"\\"
#else
#	define _END_SLASH			'/'
#	define _END_SLASH_STR		"/"
#endif


#endif