
#ifndef __STREAM_STATE_EMINEM__DEF__
#define __STREAM_STATE_EMINEM__DEF__

#include <string>

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
	StreamStateObj(StreamBuffer*buffer);
	void consume();

private:
	StreamBuffer *buffer_;
	StreamState::State state_;
	int full_length_;
	int typecode_;
	std::string payload_;
};

#endif