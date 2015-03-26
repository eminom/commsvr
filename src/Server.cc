
#include "Server.h"
#include "ProtoHandle.h"
#include "dir.pb.h"
#include "world.pb.h"


#ifdef _DIRECTORY_DIST

void ServerInitDispatcher(ProtoDispatcher& dispatcher)
{
	dispatcher.registerProto<WorldListCommand>(proto_WorldListCommand);
	dispatcher.registerProto<RegisterUserCommand>(proto_RegisterUserCommand);
}

#else

// The World Server

void ServerInitDispatcher(ProtoDispatcher& dispatcher)
{
	dispatcher.registerProto<LoginCommand>(proto_LoginCommand);
	dispatcher.registerProto<CreatePlayerCommand>(proto_CreatePlayerCommand);
}


#endif