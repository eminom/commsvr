

#include "ServerModel.h"


bool ServerModel::checkUser(const std::string &user, const std::string &passwd)
{
	return "baihai" == user &&
		"baipasswd" == passwd;
}

std::string ServerModel::tokenForAccount(const std::string &account)
{
	std::string rv;
	for(std::string::const_reverse_iterator pos=account.rbegin();pos!=account.rend();++pos){
		rv.push_back(*pos);
	}
	return rv;
}


