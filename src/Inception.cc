

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


}//End of namespace inception
