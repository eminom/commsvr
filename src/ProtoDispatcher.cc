
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ProtoDispatcher.h"

ProtoDispatcher::ProtoDispatcher()
{

}

void ProtoDispatcher::registerProto(int id, const std::string &name, BufferDecoder decoder)
{
	dist_map_[id] = name; //Override
	decoder_map_[id] = decoder;
}

void ProtoDispatcher::dispatch(int id, const std::string &buffer)
{
	DistMapIt pos = dist_map_.find(id);
	DecoderMapIt pos2 = decoder_map_.find(id);
	if(pos!=dist_map_.end() && pos2 != decoder_map_.end())
	{
		pos2->second(pos->second, buffer);
		return;
	}
	fprintf(stderr, "No decoder for id %d\n", id);
}
