
#include "Inception.h"
#include "haywire.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include "RootExplorer.h"

#define _DefaultMonitorSvrPort  11200
//#define CRLF "\r\n"

void response_complete(void* pData)
{
	printf("response_complete(%s)\n", (char*)pData);
    //std::string *pCont = static_cast<std::string*>(pData);
    //delete pCont;
}

inline void SetString(hw_string &strIn, const std::string &content)
{
	strIn.value = const_cast<char*>(content.c_str());
	strIn.length= content.size();
}

void get_fetch(http_request* request, hw_http_response* response, void *user_data)
{
	hw_string status_code;
	SETSTRING(status_code, HTTP_STATUS_200);
	hw_set_response_status_code(response, &status_code);

	hw_string content_type_name, content_type_value;
	SETSTRING(content_type_name, "Content-Type");
	SETSTRING(content_type_value, "text/html");
	hw_set_response_header(response, &content_type_name, &content_type_value);

	hw_string body;
	SETSTRING(body, "Not implemented yet");
	hw_set_body(response, &body);
	if(request->keep_alive)
	{
		hw_string keep_alive_name, keep_alive_value;
		SETSTRING(keep_alive_name, "Connection");
		SETSTRING(keep_alive_value, "Keep-Alive");
		hw_set_response_header(response, &keep_alive_name, &keep_alive_value);
	}
	else
	{
		hw_set_http_version(response, 1, 0);
	}
	hw_http_response_send(response, (void*)"get_fetch", response_complete);
}

void get_resourcepage(http_request* request, hw_http_response* response, void* user_data)
{
    hw_string status_code;
    hw_string content_type_name;
    hw_string content_type_value;
    hw_string body;
    hw_string keep_alive_name;
    hw_string keep_alive_value;
    
    SETSTRING(status_code, HTTP_STATUS_200);
    hw_set_response_status_code(response, &status_code);
    
    SETSTRING(content_type_name, "Content-Type");
    SETSTRING(content_type_value, "text/html");
    hw_set_response_header(response, &content_type_name, &content_type_value);

	std::string content;
	RootExplorer::getInstance()->retrieveContent(content);
    if(!content.size())
    {
        content = "You may get nothing, but the server is here.(exactly)";
    }
    SetString(body, content);	//~ Only 1024 (*1024 by modification)
    hw_set_body(response, &body);
    if (request->keep_alive)
    {
        SETSTRING(keep_alive_name, "Connection");
        SETSTRING(keep_alive_value, "Keep-Alive");
        hw_set_response_header(response, &keep_alive_name, &keep_alive_value);
    }
    else
    {
        hw_set_http_version(response, 1, 0);
    }
    hw_http_response_send(response, (void*)"get_resourcepage", response_complete);
}

namespace inception
{
    
int getMonitorServerPort()
{
    return _DefaultMonitorSvrPort;
}

int helloLoop(const char *serverRootDir)
{
	RootExplorer::getInstance()->setWorkingDir(serverRootDir);
	configuration config = {0};
    config.http_listen_address = "0.0.0.0";
	config.http_listen_port = _DefaultMonitorSvrPort;
	config.thread_count = 0; //~ by default
	config.parser = "http_parser";
    hw_init_with_config(&config);
    hw_http_add_route("/",      get_resourcepage, NULL);  //Is the literal string a const ? (VS nods)
	hw_http_add_route("/fetch", get_fetch, NULL);
    return hw_http_open();
}

}//end of namespace
