


#ifndef _HELLO_INCEPTION_DEF__BASIC
#define _HELLO_INCEPTION_DEF__BASIC

#include "haywire.h"

#define _PreIndexServer		"/index"
#define _PreResourcePage	"/static"
#define _PreFetch			"/fetch"
#define _PreUpload			"/upload.jsp"
#define _PreDelete			"/delete"
#define _PreGetSeed			"/seed"

#define _XxhashSeed			0x20482048

#define UnknownResStr	\
	"<font color=\"Red\">Unknown Resource Type</font>"

#define EmptyContentStr	\
	"<font color=\"Red\">You may get nothing, but the server is here.(exactly)</font>"

#define DummyContentStr \
	"<center>OH YES OH NO OH GOD</center>"

const char* fileGetStatusCode(const char *file);

void get_fetch(http_request* request, hw_http_response* response, void *user_data);
void get_upload(http_request *request, hw_http_response *response, void *user_data);
void get_indexserver(http_request *request, hw_http_response *response, void *user_data);
void get_resourcepage(http_request* request, hw_http_response* response, void* user_data);
void get_delete(http_request *req, hw_http_response *response, void*);
void get_seed(http_request *req, hw_http_response *response, void*);

#define _START_INCEPTION()	namespace inception {
#define _END_INCEPTION()	}


#endif