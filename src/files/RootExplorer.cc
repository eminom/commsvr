

#include "RootExplorer.h"
#include "DirWalker.h"
#include "utils/SlashDef.h"

#define _MinimumTimeStamp	1500

static RootExplorer *_instance = nullptr;

RootExplorer::~RootExplorer(){}
RootExplorer::RootExplorer():_timeStamp(0){
}

RootExplorer* RootExplorer::getInstance()
{
	if(!_instance)
	{
		_instance = new RootExplorer;
	}
	return _instance;
}

void RootExplorer::setWorkingDir(const std::string &d)
{
	_rootdir = d;
	if(!d.empty() && d.back() != _END_SLASH)
		_rootdir += _END_SLASH_STR;
}

const char* RootExplorer::getWorkingDir()const
{
	return _rootdir.c_str();
}

void RootExplorer::retrieveContent(std::string &out, bool doHash, unsigned int seed)
{
	time_t t = time(0);
	if( t - _timeStamp > _MinimumTimeStamp) {
		printf("Updating db\n");
		elicitDir(_rootdir.c_str(), out, doHash, seed);
		_timeStamp = t;
		contentCached_ = out; // cache
	} else {
		out = contentCached_;
	}
}


