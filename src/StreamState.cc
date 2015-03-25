
#include "StreamState.h"
#include "StreamBuffer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

StreamStateObj::StreamStateObj(StreamBuffer*buffer)
	:buffer_(buffer)
{
	state_ = StreamState::ZeroRead;
}

//Changed
void StreamStateObj::consume()
{
	bool cont = true;
	while(cont)
	{
		cont = false;
		int length = 0, typecode = -1;
		std::string str;
		StreamState::State nextState = state_; // Keep
		switch(state_){
		case StreamState::ZeroRead:
			if(buffer_->readInt32(length)){
				full_length_ = length;
				nextState = StreamState::LengthRead;
				printf("Full length expecting: %d\n", full_length_);
				//printf("pay load length is %d\n", buffer_->payLength());
				cont = true;
			}
			break;
		case StreamState::LengthRead:
			if(buffer_->readInt16(typecode)){
				typecode_ = typecode;
				nextState = StreamState::TypeCodeRead;
				printf("Type code is: %d\n", typecode_);
				//printf("payload length is %d\n", buffer_->payLength());
				cont = true;
			}
			break;
		case StreamState::TypeCodeRead:
			if(buffer_->readString(str, full_length_ - 6)){
				payload_ = str;
				//ok, time to deal with payload
				printf("Consumed\n");
				nextState = StreamState::ZeroRead;
				cont = true;
			}
			break;
		default:
			fprintf(stderr, "interesting ...%d>>\n", state_);
			abort();
			break;
		}
		state_ = nextState;
	}
}	// end of consume
