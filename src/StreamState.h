
#ifndef __STREAM_STATE_EMINEM__DEF__
#define __STREAM_STATE_EMINEM__DEF__

#include <string>
#include "ProtoDispatcher.h"

struct StreamState{
	enum State
	{
		Unknown,
		ZeroRead,
		LengthRead,
		TypeCodeRead,
		FullBodyRead,
	};
};


class StreamBuffer;
class StreamStateObj
{
public:
	StreamStateObj(StreamBuffer*buffer, client_proc_t *host);
	void consume();

private:
	void init();

private:
	StreamBuffer *buffer_;
	StreamState::State state_;
	int full_length_;
	int typecode_;
	std::string payload_;
	ProtoDispatcher dispatcher_;
	client_proc_t *host_;
};

#endif
