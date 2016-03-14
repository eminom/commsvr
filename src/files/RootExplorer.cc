

#include "RootExplorer.h"
#include "DirWalker.h"
#include "utils/SlashDef.h"

static RootExplorer *_instance = nullptr;

RootExplorer::~RootExplorer(){}
RootExplorer::RootExplorer(){}

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

void RootExplorer::retrieveContent(std::string &out, bool doHash)
{
	elicitDir(_rootdir.c_str(), out, doHash);
}


