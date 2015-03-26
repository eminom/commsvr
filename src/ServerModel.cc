

#include "ServerModel.h"


bool ServerModel::checkUser(const std::string &user, const std::string &passwd)
{
	return "baihai" == user &&
		"baipasswd" == passwd;
}


