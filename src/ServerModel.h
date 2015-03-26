

#ifndef _SERVER_MODEL_H__
#define _SERVER_MODEL_H__

#include <string>

class ServerModel
{
public:
	static bool checkUser(const std::string &user, const std::string &passwd);
	static std::string tokenForAccount(const std::string &account);
};

#endif