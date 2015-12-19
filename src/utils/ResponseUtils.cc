

#include "ResponseUtils.h"
#include <cstring>
#include <cstdlib>

#include "base/HtmlContentType.h"

#define _FileNotFoundStr	\
	"<b>File not found. Too bad</b>"

void response_complete(void* pData)
{
	printf("response_complete(%s)\n", (char*)pData);
    //std::string *pCont = static_cast<std::string*>(pData);
    //delete pCont;
}

void 
finish_response(http_request *request
	, hw_http_response *response
	, const char *statusCode
	, RespMsg *pUser
	, const char *typ
	, const char *text
	, int length)
{
	hw_string status_code;
	SETSTRING(status_code, statusCode);
	hw_set_response_status_code(response, &status_code);
	hw_string content_type_name, content_type_value;
	SETSTRING(content_type_name, "Content-Type");
	SetCString(content_type_value, typ);
	hw_set_response_header(response, &content_type_name, &content_type_value);

	//By default, all transfer are encoded in chunk
	// If not.
	if (strcmp(typ, ContentType_TextPlain))
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
	hw_http_response_send(response, pUser, pUser->getFinishCallback());
}

void 
finish_response(http_request *request
	, hw_http_response *response
	, const char *statusCode
	, RespMsg* pFinish
	, const char *typeStr
	, const char *text)
{
	finish_response(request, response, statusCode, pFinish, typeStr, text, strlen(text));
}

void 
finish_response_file(http_request *request
	, hw_http_response *response
	, const char *statusCode
	, const char *typeStr
	, const char *filepath) {
	hw_string status_code;
	SETSTRING(status_code, statusCode);
	hw_set_response_status_code(response, &status_code);

	//hw_string body;
	//body.value = (char*)text;
	//body.length = length;
	//hw_set_body(response, &body);
	if(request->keep_alive)	{
		hw_string keep_alive_name, keep_alive_value;
		SETSTRING(keep_alive_name, "Connection");
		SETSTRING(keep_alive_value, "Keep-Alive");
		hw_set_response_header(response, &keep_alive_name, &keep_alive_value);
	} else {
		hw_set_http_version(response, 1, 0);
	}

	if( !strcmp(statusCode, HTTP_STATUS_200)){
		hw_string content_type_name;
		hw_string content_type_value;
		SETSTRING(content_type_name, "Content-Type");
		SetCString(content_type_value, typeStr);
		hw_set_response_header(response, &content_type_name, &content_type_value);
		std::string msg = "Fetch for:<";
		msg += request->url;
		msg += ">";
		auto pFinish = _BuildRStr(msg);
		hw_http_response_send_file(response, pFinish, filepath, pFinish->getFinishCallback());
	} else {
		//File not found. Override the content-type
		hw_string content_type_name;
		hw_string content_type_value;
		SETSTRING(content_type_name,  "Content-Type");
		SetCString(content_type_value, ContentType_TextHtml);
		hw_set_response_header(response, &content_type_name, &content_type_value);
		hw_string body;
		body.value = _FileNotFoundStr;
		body.length = strlen(body.value);
		hw_set_body(response, &body);
		std::string msg = "Failed fetch for:<";
		msg += request->url;
		msg += ">";
		auto pFinish = _BuildRStr(msg);
		hw_http_response_send(response, pFinish, pFinish->getFinishCallback());
	}
}