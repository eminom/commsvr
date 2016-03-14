
#pragma once
#include <string>
#include <ctime>

class RootExplorer
{
private:
	RootExplorer();
	~RootExplorer();

public:
	static RootExplorer* getInstance();

public:
	void setWorkingDir(const std::string &dir);
	const char *getWorkingDir()const;
	void retrieveContent(std::string &out, bool doHash, unsigned int seed);

private:
	std::string _rootdir;
	time_t _timeStamp;
	bool _lastHashed;
	std::string contentCached_;
};
