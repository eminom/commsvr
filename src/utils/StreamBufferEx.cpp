
#include "StreamBufferEx.h"
//~
#include <boost/asio.hpp>

bool StreamBufferEx::readInt32(int &value)
{
	if(payLength() < 4){
		return false;
	}
	int v = 0, b = 1;
	for(int i=0;i<4;++i){
		v += (unsigned char)(read_ptr_[i]) * b;
		b <<= 8;
	}
	value = boost::asio::detail::socket_ops::network_to_host_long(v);
	commit(4);
	return true;
}

bool StreamBufferEx::readInt16(int &value)
{
	if(payLength() < 2){
		return false;
	}
	//printf("Debuggin readInt16 with(from low to high): %02x : %02x\n", read_ptr_[0], read_ptr_[1]);
	int v = 0, b = 1;
	for(int i=0;i<2;++i){
		v += (unsigned char)(read_ptr_[i]) * b;
		b <<= 8;
	}
	value = boost::asio::detail::socket_ops::network_to_host_short(v);
	commit(2);
	return true;
}