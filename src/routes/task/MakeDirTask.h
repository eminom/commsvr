

#ifndef _MAKE_DIR_TASK__DEF__
#define _MAKE_DIR_TASK__DEF__

#include "TaskUVBase.h"
#include <string>

class MakeDirTask:public TaskUV{
public:
	virtual ~MakeDirTask();
	MakeDirTask(const std::string &targetDir);

private:
	virtual void Proceed()override;

private:
	const std::string _targetDir;
};


#endif