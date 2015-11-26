
#include "Inception.h"
#include "haywire.h"

//#define CRLF "\r\n"


void response_complete(void* user_data)
{
	printf("response_complete(%s)\n", (char*)user_data);
}

void get_root(http_request* request, hw_http_response* response, void* user_data)
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
    
    SETSTRING(body, "hello world");
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
    
    hw_http_response_send(response, (void*)"user_data", response_complete);
}

namespace inception
{

int helloLoop() 
{
    char route[] = "/";
	configuration config = {0};
    config.http_listen_address = "0.0.0.0";
	config.http_listen_port = 11120;
	config.thread_count = 0; //~ by default
	config.parser = "http_parser";
    hw_init_with_config(&config);
    hw_http_add_route(route, get_root, NULL);
    return hw_http_open();
}

}//end of namespace