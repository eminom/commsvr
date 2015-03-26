

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
	void dispatch(int id, const std::string&, client_proc_t *host);

public:
	template<class T>
	void registerProto(BufferDecoder decoder)
	{
		dist_map_[T::ID] = T::descriptor()->full_name(); //Override
		decoder_map_[T::ID] = decoder;
	}


private:
	typedef std::map<int,std::string> DistMap;
	typedef DistMap::iterator DistMapIt;
	DistMap dist_map_;
	typedef std::map<int, BufferDecoder> DecoderMap;
	typedef DecoderMap::iterator DecoderMapIt;
	DecoderMap decoder_map_;
};




#endif
