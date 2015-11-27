
#pragma once
#include <string>

class RootExplorer
{
private:
	RootExplorer();
	~RootExplorer();

public:
	static RootExplorer* getInstance();

public:
	void setWorkingDir(const std::string &dir);
	void retrieveContent(std::string &out);

private:
	std::string _rootdir;
};
