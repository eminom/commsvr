
#ifndef _CLIENT_PROC_DEF__
#define _CLIENT_PROC_DEF__

#include "uv.h"

class StreamBuffer;
class StreamStateObj;

struct client_proc_t
{
  uv_tcp_t base;
  StreamBuffer *sb;
  StreamStateObj *so;
};

//typedef struct client_proc_s client_proc_t;


client_proc_t* createClientProcessor();
void destroyClientProcessor(client_proc_t *ptr);

void sendStreamBuffer(client_proc_t *ptr, int typecode, const char *buffer, int length);


#endif