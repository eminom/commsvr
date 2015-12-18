

#ifndef _BI_TASK_DEF__
#define _BI_TASK_DEF__

#include "TaskUVBase.h"

class BiTask:public TaskUV {
	enum{ InBiTaskCount = 2};
public:
	virtual ~BiTask();
	BiTask(TaskUV *task1, TaskUV *task2);

private:
	virtual void Proceed()override;

private:
	TaskUV * _task[InBiTaskCount];
	int _offset;
};


#endif