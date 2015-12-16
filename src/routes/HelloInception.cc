
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
#include <sys/stat.h>

#include "HelloInception.h"

const char* fileGetStatusCode(const char *file) {
	struct stat theS;
	if (stat(file, &theS) < 0) {
		return HTTP_STATUS_500;
	}
	return HTTP_STATUS_200;
}

_START_INCEPTION()
//////////////////////////////////////////////////////////
    
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
	hw_http_add_route(_PreIndexServer,  get_indexserver, NULL);
    hw_http_add_route(_PreResourcePage, get_resourcepage, NULL);  //Is the literal string a const ? (VS nods)
	hw_http_add_route(_PreFetch, get_fetch, NULL);
	hw_http_add_route(_PreUpload, get_upload, NULL);
    return hw_http_open();
}


/////////////////////////////////////////////////
_END_INCEPTION()
