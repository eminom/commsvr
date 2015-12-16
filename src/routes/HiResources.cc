
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
    if (!content.size()) {
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
