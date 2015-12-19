

#ifndef _RESP_MSG_DEF__
#define _RESP_MSG_DEF__

#include <string>
#include "haywire.h"

#define _FRM_Ctor()			const std::string &msg
#define _FRM_CtorBare()		msg

#define _BuildRStr(...)	RespMsg::__create((__VA_ARGS__))

class RespMsg {
protected:
	RespMsg(_FRM_Ctor());

public:
	static RespMsg* __create(_FRM_Ctor());
	void sendResp(hw_http_response*);
	void sendFile(hw_http_response*, const char*localFullPath);

private:
	void print();
	static void whenResponseDone(void*);

private:
	std::string msg_;
};


#endif