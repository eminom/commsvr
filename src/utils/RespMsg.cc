

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

void RespMsg::whenResponseDone(void* pData) {
	RespMsg *self = reinterpret_cast<RespMsg*>(pData);
	self->print();
	delete self;
}

void RespMsg::sendResp(hw_http_response* resp) {
	hw_http_response_send(resp, this, &RespMsg::whenResponseDone);
}

void RespMsg::sendFile(hw_http_response* resp, const char*localFullPath) {
	hw_http_response_send_file(resp,  this, localFullPath, &RespMsg::whenResponseDone);
}