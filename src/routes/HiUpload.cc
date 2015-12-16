
#include "services/Inception.h"
#include "haywire.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include "files/RootExplorer.h"
#include "utils/SlashDef.h"

//#define CRLF "\r\n"
#include "base/HtmlContentType.h"
#include "utils/ResponseUtils.h"
#include "config/HttpServerConfig.h"


void get_upload(http_request *request, hw_http_response *response, void *user_data){

	printf("upload request is being processed\n");
	if(request->body){
		printf("Content-Length: %d\n", request->body->length);
	}
	finish_response(request
		, response
		, HTTP_STATUS_500
		, (void*)"file upload request done"
		, ContentType_TextPlain
		, "Not supported for now"
		);
}