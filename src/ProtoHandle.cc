


#include "ProtoHandle.h"
#include "data.pb.h"
#include "world.pb.h"
#include "error_code.pb.h"
#include "dir.pb.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "ServerConfig.h"
#include "ServerModel.h"
#include "client_proc_t.h"

#define _ErrorParsing()\
		fprintf(stderr, "parsing error %s\n", proto.c_str());\
		fprintf(stderr, "buf length is %d\n", buf.size());\
		fprintf(stderr, "print first 4 bytes:\n");\
		for(int i=0;i<4&&i<buf.size();++i){\
			fprintf(stderr,"%02x ", buf[0]);\
		}\
		fprintf(stderr,".\n");\

void proto_WorldListCommand(const std::string &proto, const std::string &, client_proc_t* clt)
{
	//printf("On WorldListCommand:\n");
	WorldListNotify notify;
	WorldData* worldData = notify.add_world_list();
	worldData->set_host(WorldServerHost);
	worldData->set_port(WorldServerPort);
	worldData->set_id(101);
	worldData->set_name("The world No.1 server");

	std::string buffer;
	notify.SerializeToString(&buffer);
	sendCltBuf<WorldListNotify>(clt, buffer.data(), buffer.size());
	//printf("Sent !\n");
	//printf("done\n");
}

void proto_RegisterUserCommand(const std::string &proto, const std::string&buf, client_proc_t* clt)
{
	RegisterUserCommand cmd;
	if(cmd.ParseFromString(buf)){

		printf("---------Register new user--------\n");
		printf("User: %s\n", cmd.account().c_str());
		printf("Passwd:%s\n", cmd.password().c_str());
		printf("----------------------------------\n\n");

		//Granted !
		RegisterUserNotify regNotify;
		regNotify.set_exception(ET_OK);
		std::string buffer;
		regNotify.SerializeToString(&buffer);
		sendCltBuf<RegisterUserNotify>(clt, buffer.data(), buffer.size());
	} else {
		_ErrorParsing()
	}
}

void proto_LoginCommand(const std::string &proto, const std::string& buf, client_proc_t* clt)
{
	LoginCommand cmd;
	if(cmd.ParseFromString(buf)){
		printf("------------Login User--------\n");
		if(cmd.has_device_id()){
			printf("Device id: %s\n", cmd.device_id().c_str());
		}
		if(cmd.has_account()){
			printf("Acount: %s\n", cmd.account().c_str());
		}
		if(cmd.has_password()){
			printf("Password: %s\n", cmd.password().c_str());
		}
		printf("-----------------------------\n\n");

		ExceptionType result = ET_FAIL;
		if(cmd.has_account() && cmd.has_password()){
			std::string account = cmd.account();
			std::string passwd  = cmd.password();
			if(ServerModel::checkUser(account, passwd)){
				result = ET_OK;
			}
		}
		LoginNotify loginNotify;
		loginNotify.set_exception(result);
		loginNotify.set_token("201520142013");
		std::string buffer;
		loginNotify.SerializeToString(&buffer);
		sendCltBuf<LoginNotify>(clt, buffer.data(), buffer.size());
	} else {
		_ErrorParsing()
	}
}

void proto_CreatePlayerCommand(const std::string &proto, const std::string &buf, client_proc_t *clt)
{
	CreatePlayerCommand cmd;
	if(cmd.ParseFromString(buf)){
		printf("----------- CreatePlayerCommand ---------\n");
		printf("is_anonymous: %d\n", cmd.is_anonymous());
		if(cmd.has_device_id()){
			printf("device id: %s\n", cmd.device_id().c_str());
		}
		if(cmd.has_account()){
			printf("account: %s\n", cmd.account().c_str());
		}
		CreatePlayerNotify notify;
		notify.set_exception(ET_OK);
		std::string buffer;
		notify.SerializeToString(&buffer);
		sendCltBuf<CreatePlayerNotify>(clt, buffer.data(), buffer.size());
	} else {
		_ErrorParsing()
	}

}