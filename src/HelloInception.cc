
#include "Inception.h"
#include "haywire.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include "RootExplorer.h"
#include "SlashDef.h"

#define _DefaultMonitorSvrPort  11200
//#define CRLF "\r\n"

#define _PreFetch	"/fetch"

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

inline void SetCString(hw_string &strIn, const char *str)
{
	strIn.value = const_cast<char*>(str);
	strIn.length= strlen(str);
}

void finish_no_res(http_request *request, hw_http_response *response, void *user_data)
{
	hw_string status_code;
	SETSTRING(status_code, HTTP_STATUS_500);
	hw_set_response_status_code(response, &status_code);
	hw_string content_type_name, content_type_value;
	SETSTRING(content_type_name, "Content-Type");
	SETSTRING(content_type_value, "text/html");
	hw_set_response_header(response, &content_type_name, &content_type_value);
	hw_string body;
	SETSTRING(body, "Resource not found(haywire updated)");
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

void finish_with_type(http_request *request, hw_http_response *response, void *user_data, const char *typ, const char *text, int length)
{
	hw_string status_code;
	SETSTRING(status_code, HTTP_STATUS_200);
	hw_set_response_status_code(response, &status_code);
	hw_string content_type_name, content_type_value;
	SETSTRING(content_type_name, "Content-Type");
	//SETSTRING(content_type_value, "text/html");
	SetCString(content_type_value, typ);
	hw_set_response_header(response, &content_type_name, &content_type_value);

	//By default, all transfer are encoded in chunk
	// If not.
	if (strcmp(typ, "text/html"))
	{
		// It is erronous to do so.(in chunked mode.)
		//hw_string name, value;
		//SETSTRING(name, "Transfer-Encoding");
		//SETSTRING(value,"chunked");
		//hw_set_response_header(response, &name, &value);

		// It would also be erronous to do so(multiple content-length found in response, reported by Chrome)
		//hw_string name, value;
		//SETSTRING(name, "Content-Length");
		//char buf[BUFSIZ];
		//snprintf(buf, sizeof(buf), "%d", length);
		//SETSTRING(value, buf);
		//hw_set_response_header(response, &name, &value);
	}

	hw_string body;
	body.value = (char*)text;
	body.length = length;
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


bool isTextSuffix(const std::string &s)
{
	return "lua" == s || "tlog" == s || "txt" == s || "js" == s || "py" == s || "pl" == s;
}

bool isImageSuffix(const std::string &s)
{
	return "png" == s || "jpg" == s || "jpeg" == s;
}

void get_fetch(http_request* request, hw_http_response* response, void *user_data)
{
	bool processed = false;
	const char *url = request->url;
	size_t lz = strlen(_PreFetch);
	if(strlen(url) >= lz + 1 && !strncmp(_PreFetch, url, lz)  && *(url+(lz+1)))
	{
		const char *rest = url + (lz + 1);
		std::string cwd = RootExplorer::getInstance()->getWorkingDir();
		if(cwd.size()>0 && cwd[cwd.size()-1] != _END_SLASH)
		{
			cwd += _END_SLASH_STR;
		}
		std::string request_path = cwd + rest;
		const char* p = strrchr(rest, '/');
		const char* q = strrchr(rest, '\\');
		if ( !p || (p && q && p < q ) )
		{
			p = q;
		}
		const char* d = strrchr(rest, '.');
		std::string suffix;
		if (d && (!p || d > p) )
		{
			suffix = d + 1;
		}
		for(auto &ch:suffix)
		{
			ch = tolower(ch);
		}
		if(isTextSuffix(suffix))
		{
			FILE *fin = fopen(request_path.c_str(), "r");
			if(fin)
			{
				fseek(fin, 0, SEEK_END);
				size_t sz = ftell(fin);
				rewind(fin);
				char *buffer = (char*)malloc((sz+1)*sizeof(char));
				memset(buffer, 0, sz+1);
				fread(buffer, 1, sz, fin);
				buffer[sz] = 0;
				fclose(fin);
				finish_with_type(request, response, user_data, "text/html", buffer, sz);
				free(buffer);
				processed = true;
			}
		}
		else if(isImageSuffix(suffix))
		{
			FILE *fin = fopen(request_path.c_str(), "rb");
			if(fin)
			{
				fseek(fin, 0, SEEK_END);
				size_t sz = ftell(fin);
				rewind(fin);
				char *buffer = (char*)malloc((sz+1)*sizeof(char));
				memset(buffer, 0, sz+1);
				fread(buffer, 1, sz, fin);  //~ and it returns "sz". 
				//buffer[sz] = 0;
				fclose(fin);
				std::string mimeType = "image/";
				mimeType += suffix;
				finish_with_type(request, response, user_data, mimeType.c_str(), buffer, sz);
				free(buffer);
				processed = true;
			}
		}
		else
		{
			const char *word = "Unknown type";
			finish_with_type(request, response, user_data, "text/html", word, strlen(word));
			processed = true;
		}
	}

	//Or else: resource not found
	if(!processed)
	{
		finish_no_res(request, response, user_data);
	}
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
	hw_http_add_route(_PreFetch, get_fetch, NULL);
    return hw_http_open();
}

}//end of namespace
