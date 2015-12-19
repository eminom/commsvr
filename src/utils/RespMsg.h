

#ifndef _RESP_MSG_DEF__
#define _RESP_MSG_DEF__

#include <string>
#include "haywire.h"

#define _FRM_Ctor()			const std::string &msg
#define _FRM_CtorBare()		msg

#define _BuildRStr(...)	RespMsg::__create((__VA_ARGS__))

class RespMsg
{
protected:
	RespMsg(_FRM_Ctor());
public:
	void print();
	http_response_complete_callback getFinishCallback();
	static RespMsg* __create(_FRM_Ctor());

private:
	static void whenResponseDone(void*);

private:
	std::string msg_;
};


#endif