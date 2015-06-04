


#include "ProtoHandle.h"

#include "cs_world.pb.h"
#include "cs_dir.pb.h"

#include "data.pb.h"
#include "exceptiontype.pb.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ServerConfig.h"
#include "ServerModel.h"

#include "client_proc_t.h"

#define _ErrorParsing()\
		fprintf(stderr, "parsing error %s\n", proto.c_str());\
		fprintf(stderr, "buf length is %lu\n", buf.size());\
		fprintf(stderr, "print first 4 bytes:\n");\
		for(int i=0;i<4&&i<buf.size();++i){\
			fprintf(stderr,"%02x ", buf[0]);\
		}\
		fprintf(stderr,".\n");\

void proto_WorldListCommand(const std::string &proto, const std::string &, client_proc_t* clt)
{
	//printf("On WorldListCommand:\n");
	ResponseWorldList response;
	Data_WorldInfo* data = response.add_world_list();
	data->set_host(WorldServerHost);
	data->set_port(WorldServerPort);
	data->set_id(101);
	data->set_name("The world No.1 server");

	std::string buffer;
	response.SerializeToString(&buffer);
	sendCltBuf<ResponseWorldList>(clt, buffer.data(), buffer.size());
	//printf("Sent !\n");
	//printf("done\n");
}

void proto_RegisterUserCommand(const std::string &proto, const std::string&buf, client_proc_t* clt)
{
	RequestRegisterUser req;
	if(req.ParseFromString(buf)){

		printf("---------Register new user--------\n");
		printf("User: %s\n", req.account().c_str());
		printf("Passwd:%s\n", req.password().c_str());
		printf("----------------------------------\n\n");

		//Granted !
		ResponseUserRegister response;
		response.set_exception(ET_OK);
		std::string buffer;
		response.SerializeToString(&buffer);
		sendCltBuf<ResponseUserRegister>(clt, buffer.data(), buffer.size());
	} else {
		_ErrorParsing()
	}
}

void proto_LoginCommand(const std::string &proto, const std::string& buf, client_proc_t* clt)
{
	RequestLogin req;
	if(req.ParseFromString(buf)){
		printf("------------Login User--------\n");
		if(req.has_device_id()){
			printf("Device id: %s\n", req.device_id().c_str());
		}
		if(req.has_account()){
			printf("Acount: %s\n", req.account().c_str());
		}
		if(req.has_password()){
			printf("Password: %s\n", req.password().c_str());
		}
		printf("-----------------------------\n\n");

		ExceptionType result = ET_FAIL;
		if(req.has_account() && req.has_password()){
			std::string account = req.account();
			std::string passwd  = req.password();
			if(ServerModel::checkUser(account, passwd)){
				result = ET_OK;
			}
		}
		ResponseLogin response;
		response.set_exception(result);
		response.set_token(ServerModel::tokenForAccount(req.has_account()? req.account():""));
		std::string buffer;
		response.SerializeToString(&buffer);
		sendCltBuf<ResponseLogin>(clt, buffer.data(), buffer.size());
	} else {
		_ErrorParsing()
	}
}

void proto_CreatePlayerCommand(const std::string &proto, const std::string &buf, client_proc_t *clt)
{
	RequestCreatePlayer req;
	if(req.ParseFromString(buf)){
		printf("----------- CreatePlayerCommand ---------\n");
		printf("is_anonymous: %d\n", req.is_anonymous());
		if(req.has_device_id()){
			printf("device id: %s\n", req.device_id().c_str());
		}
		if(req.has_account()){
			printf("account: %s\n", req.account().c_str());
		}
		ResponseCreatePlayer response;
		response.set_exception(ET_OK);
		std::string buffer;
		response.SerializeToString(&buffer);
		sendCltBuf<ResponseCreatePlayer>(clt, buffer.data(), buffer.size());
	} else {
		_ErrorParsing()
	}

}