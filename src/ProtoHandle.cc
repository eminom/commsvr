


#include "ProtoHandle.h"
#include "data.pb.h"
#include "world.pb.h"
#include "error_code.pb.h"
#include "dir.pb.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "client_proc_t.h"

void proto_WorldListCommand(const std::string &proto, const std::string &, client_proc_t* clt)
{
	//printf("On WorldListCommand:\n");
	WorldListNotify notify;
	WorldData* worldData = notify.add_world_list();
	worldData->set_host("192.168.2.23");
	worldData->set_port(12000);
	worldData->set_id(101);
	worldData->set_name("The world No.1 server");

	std::string buffer;
	notify.SerializeToString(&buffer);
	sendStreamBuffer(clt, 2, buffer.data(), buffer.size());
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
		sendStreamBuffer(clt, 4, buffer.data(), buffer.size());
	} else {
		fprintf(stderr, "parsing error for Register-user-cmd\n");
		fprintf(stderr, "buf length is %d\n", buf.size());
		fprintf(stderr, "print first 4 bytes:\n");
		for(int i=0;i<4&&i<buf.size();++i){
			fprintf(stderr,"%02x ", buf[0]);
		}
		fprintf(stderr,"\n");
	}
}

