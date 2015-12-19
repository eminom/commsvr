
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>

#include "haywire.h"
#include "HelloInception.h"
#include "files/RootExplorer.h"
#include "utils/ResponseUtils.h"
#include "task/DeleteTask.h"

//#include "config/HttpServerConfig.h"
#include "base/HtmlContentType.h"

void get_delete(http_request *req, hw_http_response *response, void*) {
	bool processed = false;
	const char *url = req->url;
	const size_t lz = strlen(_PreDelete);
	if(strlen(url) >= lz + 1 
		&& '/' == url[lz]
		&& !strncmp(_PreDelete, url, lz)  
		&& *(url+(lz+1))) {

		const char *rest = url + (lz + 1);
		std::string cwd = RootExplorer::getInstance()->getWorkingDir();
		std::string target_path = cwd + rest;
		queueSimpleTaskUV(new DeleteTask(target_path, url));

		const char* statusCode = fileGetStatusCode(target_path.c_str());

		const char *localFb = 0, *respText = 0;
		if( !strcmp(statusCode, HTTP_STATUS_200) ){
			localFb = "Delete one file";
			respText= "Delete done";
		} else {
			localFb = "Delete non-existen";
			respText= "Delete failed";
		}

		finish_response(req,
			response,
			statusCode,
			_BuildRStr(localFb),
			ContentType_TextPlain,
			respText
		);
	} else {
		const char *msg = "<font color=\"red\">Invalid format for /delete</font>";
		finish_response(req
			, response
			, HTTP_STATUS_500
			, _BuildRStr("Invalid delete format")
			, ContentType_TextHtml
			, msg
			, strlen(msg)
			);
	}


}