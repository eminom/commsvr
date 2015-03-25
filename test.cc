
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "pb/data.pb.h"

int main()
{
	WorldData wd;
	wd.set_host("192.168.2.2");
	wd.set_port(12000);
	wd.set_id(230);
	wd.set_name("Eminem");

	std::string str;
	wd.SerializeToString(&str);

	printf("Done\n");

	
}
