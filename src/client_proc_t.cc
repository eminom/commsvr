
#include <cstdlib>
#include <cstdio>
#include <cstring>
//#include "StreamBuffer.h"
//#include "StreamState.h"
#include "client_proc_t.h"
#include "HeaderLengthStyle.h"
#include "uv.h"

//#include <boost/asio.hpp>

extern uv_loop_t *loop;

client_proc_t* createClientProcessor()
{
    client_proc_t* rv = (client_proc_t*)malloc(sizeof(client_proc_t));
	memset(rv,0,sizeof(*rv));
    uv_tcp_init(loop, (uv_tcp_t*)rv);
#if 0
    rv->sb = new StreamBuffer;
    rv->so = new StreamStateObj(rv->sb, rv);
#endif
    return rv;
}

void onHandlerClosed(uv_handle_t *handler)
{
	free(handler);
}

void destroyClientProcessor(client_proc_t *ptr)
{
    uv_close((uv_handle_t*)ptr, onHandlerClosed);
    delete ptr->so;
    delete ptr->sb;
	ptr->so = NULL;
	ptr->sb = NULL;
    //free(ptr); //It must wait.
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    } else {
        // printf("written to client\n");
    }
    free(req);
}

#if 0
void _fillHeader(char *buffer, int final_length, int typecode)
{
    int write_length = boost::asio::detail::socket_ops::host_to_network_long(final_length + _LengthFix);
    *((int*)buffer) = write_length;     //~ x86
    int tc = boost::asio::detail::socket_ops::host_to_network_long(typecode);
    *((int*)(buffer + 4)) = tc;
}

void _sendStreamBuffer(client_proc_t *ptr, int typecode, const char *buffer, int length)
{
    uv_stream_t *stream = (uv_stream_t*)ptr;
    uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
    int head_length = 8;   //~ Length and the typecode.
    int final_length = length + head_length;
    char *ex_buffer = (char*)malloc(final_length);
    _fillHeader(ex_buffer, final_length, typecode);
    if (length>0) {
        memcpy(ex_buffer + head_length, buffer, length);
    }
    uv_buf_t wrbuf = uv_buf_init(ex_buffer, final_length);
    uv_write(req, stream, &wrbuf, 1, echo_write);
    //printf("Something is sent to client(%d)\n", final_length);
    free(ex_buffer);
}
#endif