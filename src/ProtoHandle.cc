


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
#include "XAssert.h"
#include "cJSON.h"

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
	FILE *fin = fopen("conf.json","rb");
	if(!fin){
		fprintf(stderr, "Cannot read conf for world server");
		abort();
	}
	fseek(fin, 0, SEEK_END);
	int sz = (int)ftell(fin);
	char *buf = (char*)malloc(sizeof(char)*(sz+1));
	memset(buf, 0, sizeof(char)*(sz + 1));
	rewind(fin);
	fread(buf, sizeof(char), sz, fin);
	fclose(fin);

	cJSON *root = cJSON_Parse(buf);
	cJSON *wd = cJSON_GetObjectItem(root, "world");
	if(wd){
		XAssert( wd->type == cJSON_Array, "Must be array for world list");
		for(int i=0;i<cJSON_GetArraySize(wd);++i){
			cJSON *one = cJSON_GetArrayItem(wd, i);
			
			XAssert( one->type == cJSON_Object, "Must be object for a single entry");
			Data_WorldInfo* data = response.add_world_list();

			cJSON *ip = cJSON_GetObjectItem(one, "ip");
			XAssert(cJSON_String == ip->type, "Must be string");
			cJSON *port = cJSON_GetObjectItem(one, "port");
			XAssert(cJSON_Number == port->type, "Must be number for port");
			cJSON *id = cJSON_GetObjectItem(one, "id");
			XAssert(cJSON_Number == id->type, "Must be number for id");
			cJSON *name = cJSON_GetObjectItem(one, "name");
			XAssert(cJSON_String == name->type, "Must be string for name");

			data->set_host(ip->valuestring);
			data->set_port(port->valueint);
			data->set_id(id->valueint);
			data->set_name(name->valuestring);
		}
	}
	cJSON_Delete(root);
	free(buf);

	std::string buffer;
	response.SerializeToString(&buffer);
	sendCltBuf<ResponseWorldList>(clt, buffer.data(), buffer.size());
	//printf("Sent !\n");
	//printf("done\n");
}

void proto_RegisterUserCommand(const std::string &proto, const std::string&buf, client_proc_t* clt)
{
	RequestUserRegister req;
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