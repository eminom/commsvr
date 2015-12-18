


#ifndef _UPLOAD_TASK_DEF__
#define _UPLOAD_TASK_DEF__

#include "haywire.h"
#include <string>
#include "MyUVTask.h"

class UploadTask:public TaskUV
{
public:
	virtual ~UploadTask();

public:
	UploadTask(hw_string*, const std::string &full);
	virtual void Proceed()override;

private:
	hw_string *_body;
	std::string _finalpath;

private:
	UploadTask(const UploadTask&);
	void operator=(const UploadTask&);
};

#endif