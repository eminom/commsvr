
#include "Server.h"
#include "ProtoHandle.h"

#ifndef _WIN32

#include "cs_dir.pb.h"
#include "cs_world.pb.h"


#ifdef _DIRECTORY_DIST

void ServerInitDispatcher(ProtoDispatcher& dispatcher)
{
	dispatcher.registerProto<RequestWorldList>(proto_WorldListCommand);
	dispatcher.registerProto<RequestUserRegister>(proto_RegisterUserCommand);
}

#else

// The World Server

void ServerInitDispatcher(ProtoDispatcher& dispatcher)
{
	dispatcher.registerProto<RequestLogin>(proto_LoginCommand);
	dispatcher.registerProto<RequestCreatePlayer>(proto_CreatePlayerCommand);
}


#endif


#else  //Else win32

void ServerInitDispatcher(ProtoDispatcher& dispatcher)
{
	//dispatcher.registerProto<RequestLogin>(proto_LoginCommand);
	//dispatcher.registerProto<RequestCreatePlayer>(proto_CreatePlayerCommand);
}

#endif