
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ProtoDispatcher.h"

ProtoDispatcher::ProtoDispatcher()
{

}

void ProtoDispatcher::dispatch(int id, const std::string &buffer, client_proc_t *host)
{
	DistMapIt pos = dist_map_.find(id);
	DecoderMapIt pos2 = decoder_map_.find(id);
	if(pos!=dist_map_.end() && pos2 != decoder_map_.end())
	{
		pos2->second(pos->second, buffer, host);
		return;
	}
	fprintf(stderr, "No decoder for id %d\n", id);
}
