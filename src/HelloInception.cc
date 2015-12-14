
#include "Inception.h"
#include "haywire.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include "RootExplorer.h"
#include "SlashDef.h"

//#define CRLF "\r\n"
#include "base/HtmlContentType.h"
#include "utils/ResponseUtils.h"
#include "config/HttpServerConfig.h"

#define _PreFetch	"/fetch"

#include <sys/stat.h>

#define UnknownResStr	\
	"<font color=\"Red\">Unknown Resource Type</font>"

#define EmptyContentStr	\
	"<font color=\"Red\">You may get nothing, but the server is here.(exactly)</font>"

const char* fileGetStatusCode(const char *file) {
	struct stat theS;
	if( stat(file, &theS) < 0 ){
		return HTTP_STATUS_500;
	}
	return HTTP_STATUS_200;
}

void get_fetch(http_request* request, hw_http_response* response, void *user_data) {
	const char *url = request->url;
	size_t lz = strlen(_PreFetch);
	while(strlen(url) >= lz + 1 && !strncmp(_PreFetch, url, lz)  && *(url+(lz+1))) {
		const char *rest = url + (lz + 1);
		std::string cwd = RootExplorer::getInstance()->getWorkingDir();
		if(cwd.size()>0 && cwd[cwd.size()-1] != _END_SLASH)	{
			cwd += _END_SLASH_STR;
		}
		std::string request_path = cwd + rest;
		const char* p = strrchr(rest, '/');
		const char* q = strrchr(rest, '\\');
		if ( !p || (p && q && p < q ) )	{
			p = q;
		}
		const char* d = strrchr(rest, '.');
		std::string suffix;
		if (d && (!p || d > p) ) {
			suffix = d + 1;
		}
		for(auto &ch:suffix) {
			ch = tolower(ch);
		}
		if(isPlainTextSuffix(suffix)) {
			finish_response_file(request
				, response
				, fileGetStatusCode(request_path.c_str())
				, (void*)"text file transfer"
				, ContentType_TextPlain
				, request_path.c_str()
				);
			break;
		} else if(isImageSuffix(suffix)) {
			std::string mimeType = getImageMimeType(suffix);
			finish_response_file(request
				, response
				, fileGetStatusCode(request_path.c_str())
				, (void*)"file transfer"
				, mimeType.c_str()
				, request_path.c_str()
				);
			break;
		} else {
			//const char *word = UnknownResStr;
			//finish_response(request, response, HTTP_STATUS_500, "void(Unknown)", ContentType_TextHtml, word, strlen(word));
			//break;
			finish_response_file(request
				, response
				, fileGetStatusCode(request_path.c_str())
				, (void*)"unknown type file transfer"
				, ContentType_BinaryStream
				, request_path.c_str()
				);
			break;
		}
		//~ Finally
		const char *msg = "Ambiguous Request";
		finish_response(request, response, HTTP_STATUS_500, (void*)"void()", ContentType_TextPlain, msg, strlen(msg));
		break;
	}
}

void get_resourcepage(http_request* request, hw_http_response* response, void* user_data) {
    hw_string status_code;
    hw_string content_type_name;
    hw_string content_type_value;
    hw_string body;
    hw_string keep_alive_name;
    hw_string keep_alive_value;
    
    SETSTRING(status_code, HTTP_STATUS_200);
    hw_set_response_status_code(response, &status_code);
    
    SETSTRING(content_type_name, "Content-Type");
    SETSTRING(content_type_value, ContentType_TextHtml);
    hw_set_response_header(response, &content_type_name, &content_type_value);

	std::string content;
	RootExplorer::getInstance()->retrieveContent(content);
    if(!content.size()) {
        content = EmptyContentStr;
    }
    SetString(body, content);	//~ Only 1024 (*1024 by modification)
    hw_set_body(response, &body);
    if (request->keep_alive) {
        SETSTRING(keep_alive_name, "Connection");
        SETSTRING(keep_alive_value, "Keep-Alive");
        hw_set_response_header(response, &keep_alive_name, &keep_alive_value);
    } else {
        hw_set_http_version(response, 1, 0);
    }
    hw_http_response_send(response, (void*)"get_resourcepage", response_complete);
}

namespace inception
{
    
int getMonitorServerPort() {
    return _DefaultMonitorSvrPort;
}

int httpStaticFileLoop(const char *serverRootDir) {
	RootExplorer::getInstance()->setWorkingDir(serverRootDir);
	configuration config = {0};
    config.http_listen_address = "0.0.0.0";
	config.http_listen_port = _DefaultMonitorSvrPort;
	config.thread_count = 0; //~ by default
	config.parser = "http_parser";
    hw_init_with_config(&config);
    hw_http_add_route("/",      get_resourcepage, NULL);  //Is the literal string a const ? (VS nods)
	hw_http_add_route(_PreFetch, get_fetch, NULL);
    return hw_http_open();
}

}//end of namespace
