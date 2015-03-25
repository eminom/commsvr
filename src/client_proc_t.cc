
#include <cstdlib>
#include "StreamBuffer.h"
#include "StreamState.h"
#include "client_proc_t.h"


extern uv_loop_t *loop;

client_proc_t* createClientProcessor()
{
    client_proc_t* rv = (client_proc_t*)malloc(sizeof(client_proc_t));
    uv_tcp_init(loop, (uv_tcp_t*)rv);
    rv->sb = new StreamBuffer;
    rv->so = new StreamStateObj(rv->sb, rv);
    return rv;
}

void destroyClientProcessor(client_proc_t *ptr)
{
    uv_close((uv_handle_t*)ptr, 0);
    delete ptr->so;
    delete ptr->sb;
    free(ptr);
}
