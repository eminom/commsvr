
#include "ProtoHandle.h"
#include "data.pb.h"
#include "world.pb.h"
#include "error_code.pb.h"
#include "dir.pb.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

void proto_WorldListCommand(const std::string &proto, const std::string &, client_proc_t*)
{
	printf("On WorldListCommand:\n");
	printf("done\n");
}

void proto_RegisterUserCommand(const std::string &proto, const std::string&buf, client_proc_t*)
{
	RegisterUserCommand cmd;
	if(cmd.ParseFromString(buf)){
		printf("User: %s\n", cmd.account().c_str());
		printf("Passwd:%s\n", cmd.password().c_str());
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

