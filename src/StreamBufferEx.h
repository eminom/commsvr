

#ifndef _STREAM_BUFFER_EX_DEF__
#define _STREAM_BUFFER_EX_DEF__

#include "StreamBuffer.h"

class StreamBufferEx:public StreamBuffer
{
public:
	StreamBufferEx(const char *buffer, int length);
	StreamBufferEx();
	
public:
	bool readInt32(int &value);
	bool readInt16(int &value);
};


#endif