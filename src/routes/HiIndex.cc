
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

void get_indexserver(http_request *request, hw_http_response *response, void *user_data) {
	std::string cwd = RootExplorer::getInstance()->getWorkingDir();
	std::string rq_path = cwd + "index.html";
	finish_response_file(request
		, response
		, fileGetStatusCode(rq_path.c_str())
		, (void*)"index page"
		, ContentType_TextHtml
		, rq_path.c_str()
		);
}
