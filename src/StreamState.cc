
#include "StreamState.h"
#include "StreamBuffer.h"
#include "ProtoDispatcher.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "ProtoHandle.h"

StreamStateObj::StreamStateObj(StreamBuffer*buffer)
	:buffer_(buffer)
{
	state_ = StreamState::ZeroRead;
	init();
}

void StreamStateObj::init()
{
	dispatcher_.registerProto(1, "WorldListCommand", proto_WorldListCommand);
	dispatcher_.registerProto(3, "RegisterUserCommand", proto_RegisterUserCommand);
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
				//printf("Full length expecting: %d\n", full_length_);
				//printf("pay load length is %d\n", buffer_->payLength());
				cont = true;
			}
			break;
		case StreamState::LengthRead:
			if(buffer_->readInt16(typecode)){
				typecode_ = typecode;
				nextState = StreamState::TypeCodeRead;
				//printf("Type code is: %d\n", typecode_);
				//printf("payload length is %d\n", buffer_->payLength());
				cont = true;
			}
			break;
		case StreamState::TypeCodeRead:
			if(buffer_->readString(str, full_length_ - 6)){
				payload_ = str;
				//ok, time to deal with payload
				//printf("Consumed\n");
				dispatcher_.dispatch(typecode_, payload_);
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
