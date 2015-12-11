


#ifndef _RESPONSE_UTILS_DEF__
#define _RESPONSE_UTILS_DEF__

#include "haywire.h"
#include <string>
#include <cstring>

void finish_response(http_request *request
	, hw_http_response *response
	, const char *statusCode
	, void *user_data
	, const char *typeStr
	, const char *text
	, int length);

void finish_response(http_request *request
	, hw_http_response *response
	, const char *statusCode
	, void *user_data
	, const char *typeStr
	, const char *text);


void 
finish_response_file(http_request *request
	, hw_http_response *response
	, const char *statusCode
	, void *user_data
	, const char *typeStr
	, const char *filepath);

void response_complete(void* pData);

inline void SetString(hw_string &strIn, const std::string &content)
{
	strIn.value = const_cast<char*>(content.c_str());
	strIn.length= content.size();
}

inline void SetCString(hw_string &strIn, const char *str)
{
	strIn.value = const_cast<char*>(str);
	strIn.length= strlen(str);
}


#endif
