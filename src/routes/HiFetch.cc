
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

#include "HelloInception.h"
#include <algorithm>


void get_fetch(http_request* request, hw_http_response* response, void *user_data) {
	bool processed = false;
	const char *url = request->url;
	size_t lz = strlen(_PreFetch);
	if(strlen(url) >= lz + 1 && !strncmp(_PreFetch, url, lz)  && *(url+(lz+1))) {
		const char *rest = url + (lz + 1);
		std::string cwd = RootExplorer::getInstance()->getWorkingDir();
		std::string request_path = cwd + rest;
		const char* p = strrchr(rest, '/');
		const char* q = strrchr(rest, '\\');
		if ( !p || (p && q && p < q ) )
			p = q;
		const char* d = strrchr((p?p+1:rest), '.');
		std::string suffix = d?d+1:"";
		std::for_each(suffix.begin(), suffix.end(), [](char &ch){ch=tolower(ch);});
		std::string mimeType;
		bool found = getFileMimeType(suffix, mimeType);
		finish_response_file(request
			, response
			, fileGetStatusCode(request_path.c_str())
			, (void*)"file transfer"
			, mimeType.c_str()
			, request_path.c_str()
			);
	} else {
		const char *msg = DummyContentStr;
		finish_response(request
			, response
			, HTTP_STATUS_500
			, (void*)"void()"
			, ContentType_TextHtml
			, msg
			, strlen(msg)
			);
	}
}