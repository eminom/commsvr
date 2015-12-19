

#include "RespMsg.h"

RespMsg* RespMsg::__create(_FRM_Ctor()) {
	return new RespMsg(_FRM_CtorBare());
}

RespMsg::RespMsg(const std::string &inMsg)
	:msg_(inMsg){
}

void RespMsg::print() {
	printf("%s\n", msg_.c_str());
}

http_response_complete_callback 
RespMsg::getFinishCallback() {
	return &RespMsg::whenResponseDone;
}

void RespMsg::whenResponseDone(void* pData) {
	RespMsg *self = reinterpret_cast<RespMsg*>(pData);
	self->print();
	delete self;
}
