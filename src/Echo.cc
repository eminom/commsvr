
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <string>
#include <cassert>

#include "StreamBuffer.h"
#include "StreamState.h"
#include "EnvConfig.h"

// StreamBuffer gStreamBuffer;
// StreamStateObj gStateObj(&gStreamBuffer);

#ifdef _DIRECTORY_DIST
#   define DEFAULT_PORT 11000
#else
#   define DEFAULT_PORT 12000
#endif


#define DEFAULT_BACKLOG 128

#include "client_proc_t.h"

//Global
uv_loop_t *loop;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        //uv _close((uv_handle_t*) client, NULL);
        destroyClientProcessor((client_proc_t*)client);
    } else if (nread > 0) {
			  //printf("Read for %d\n", nread);
        client_proc_t *clt = (client_proc_t*)client;
		clt->sb->append(buf->base, nread);
		clt->so->consume();
        //uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        //uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
        //uv_write(req, client, &wrbuf, 1, echo_write);
    }

    if (buf->base)
        free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    // uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    // uv_tcp_init(loop, client);
    client_proc_t *client = createClientProcessor();
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
				printf("New connection made\n");
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
                printf("Starting read\n");
				printf("Starting read\n");
    }
    else {
        // uv _close((uv_handle_t*) client, NULL);
        destroyClientProcessor(client);
    }
}

int main() {

#ifdef _DIRECTORY_DIST
    printf("Directory server starting ... \n");
#else
    printf("World server starting ... \n");
#endif
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
