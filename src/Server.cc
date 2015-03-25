
#include "Server.h"
#include "ProtoHandle.h"
#include "dir.pb.h"
#include "world.pb.h"


#ifdef _DIRECTORY_DIST

void ServerInitDispatcher(ProtoDispatcher& dispatcher)
{
	dispatcher.registerProto(1, "WorldListCommand", proto_WorldListCommand);
	dispatcher.registerProto(3, "RegisterUserCommand", proto_RegisterUserCommand);
}

#else

// The World Server

void ServerInitDispatcher(ProtoDispatcher& dispatcher)
{
	 dispatcher.registerProto(1, "LoginCommand", proto_LoginCommand);
}


#endif