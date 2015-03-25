

#ifndef _PROTO_DISPATCHER_DEF__
#define _PROTO_DISPATCHER_DEF__

#include <string>
#include <map>
#include <list>

struct client_proc_t;

typedef void(*BufferDecoder)(const std::string &proto, const std::string&, client_proc_t *host);

class ProtoDispatcher
{
public:
	ProtoDispatcher();
	void registerProto(int id, const std::string &name, BufferDecoder);
	void dispatch(int id, const std::string&, client_proc_t *host);

private:
	typedef std::map<int,std::string> DistMap;
	typedef DistMap::iterator DistMapIt;
	DistMap dist_map_;
	typedef std::map<int, BufferDecoder> DecoderMap;
	typedef DecoderMap::iterator DecoderMapIt;
	DecoderMap decoder_map_;
};




#endif
