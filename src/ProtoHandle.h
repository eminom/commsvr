

#ifndef _PROTO_HANDLES_DEF__
#define _PROTO_HANDLES_DEF__

#include <string>
struct client_proc_t;

//~  Directory server
void proto_WorldListCommand(const std::string &proto, const std::string &, client_proc_t*);
void proto_RegisterUserCommand(const std::string &proto, const std::string&, client_proc_t*);
void proto_LoginCommand(const std::string &proto, const std::string&, client_proc_t*);
//~World server

#endif
