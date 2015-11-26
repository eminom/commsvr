

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <cassert>
#include <ctime>
#include <uv.h>

#include "StreamBuffer.h"
#include "StreamState.h"
#include "EnvConfig.h"
#include "ServerConfig.h"

extern "C"{
#include "haywire.h"
}

#define DEFAULT_BACKLOG 128
#include "client_proc_t.h"

#include "Inception.h"

//Gobal: literally.
uv_loop_t *loop;
static InceptionCallback gCallback = [](const std::string&){};

class CmdStringBuffer
{
private:
	CmdStringBuffer();
	~CmdStringBuffer();

public:
	static CmdStringBuffer* getInstance();
	void imbibe(const char *start, int length);

private:
	StreamBuffer *const _buffer;

private:
	CmdStringBuffer(const CmdStringBuffer&);
	void operator=(const CmdStringBuffer&);
};

CmdStringBuffer::~CmdStringBuffer()
{
	delete _buffer;
}

CmdStringBuffer::CmdStringBuffer()
	:_buffer(new StreamBuffer)
{

}

static CmdStringBuffer *_cmd;

CmdStringBuffer* CmdStringBuffer::getInstance()
{
	if( ! _cmd )
	{
		_cmd = new CmdStringBuffer;
	}
	return _cmd;
}

void CmdStringBuffer::imbibe(const char *start, int length)
{
	int cur = 0, pre = 0;
	while ( cur < length )
	{
		if('\n' == start[cur])
		{
			if (cur - pre - 1 > 0)
			{
				//~ - 1 removes the last \r(if you test with telnet)
				_buffer->append(start + pre, cur - pre - 1);
				std::string content;
				if( _buffer->readString(content) )
				{
					gCallback(content);
				}
			}
			cur = cur + 1;
			pre = cur;
		}
		else
		{
			cur++;
		}
	}
	//Length of 0 is acceptable.
	_buffer->append(start + pre, cur - pre);
}

namespace inception{

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) 
{
	buf->base = (char*) malloc(suggested_size);
	buf->len = suggested_size;
}

void echo_write_simple(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    } else {
        // printf("written to client\n");
    }
    free(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	if (nread < 0) {
		if (nread != UV_EOF){
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		}
		//uv-close((uv_handle_t*) client, NULL);
		destroyClientProcessor((client_proc_t*)client);
	} else if (nread > 0) {
		//printf("Read for %d\n", nread);
		CmdStringBuffer::getInstance()->imbibe(buf->base, nread);

		//How to response.
#if 1  //_WIN32
		uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
		uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
		uv_write(req, client, &wrbuf, 1, echo_write_simple);  //buf is to be released soon.
#else
		client_proc_t *clt = (client_proc_t*)client;
		clt->sb->append(buf->base, nread);
		clt->so->consume();
#endif
	}

	if (buf->base)
		free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
	if (status < 0) {
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		return;
	}

	// uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
	// uv_tcp_init(loop, client);
	client_proc_t *client = createClientProcessor();
	if (uv_accept(server, (uv_stream_t*) client) == 0) {
		uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
	} else {
		//uv-close((uv_handle_t*) client, NULL);
		destroyClientProcessor(client);
	}
}

int masterLoop(const InceptionCallback& cb) {
	gCallback = cb;
	loop = uv_default_loop();
	uv_tcp_t server;
	uv_tcp_init(loop, &server);
	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
	if (r) {
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}


#define CRLF "\r\n"

void response_complete(void* user_data)
{
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

int helloLoop() 
{
    char route[] = "/";
	configuration config = {0};
    config.http_listen_address = "0.0.0.0";
	config.http_listen_port = 8083;
	config.thread_count = 0; //~ by default
	config.parser = "http_parser";
    hw_init_with_config(&config);
    hw_http_add_route(route, get_root, NULL);
    return hw_http_open();
}

/* Locate the 
 * non-internal
 * IPv4 family
 * name which does not start with Virtual
 */ 
std::string retrieveLocalIP() {
	std::string rv = "127.0.0.1"; //Localhost for default.
	uv_interface_address_t *info = nullptr;
	int count = 0;
	uv_interface_addresses(&info, &count);
	for (int i=count-1;i>=0;--i) {
		const uv_interface_address_t &intf = info[i];
		if(intf.is_internal || !strncmp(intf.name, "Virtual", 7)){
			continue;
		}
		if (intf.address.address4.sin_family == AF_INET) {
			char buf[512];
			uv_ip4_name(&intf.address.address4, buf, sizeof(buf));
			rv = buf;
			break;
		}
	}
	uv_free_interface_addresses(info, count);
	return rv;
}

}//End of namespace inception
