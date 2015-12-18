

#include "BiTask.h"


void on_bi_task_start(uv_work_t *req){

}

void on_bi_task_done(uv_work_t *req){

}


BiTask::~BiTask()
{

}

BiTask::BiTask(TaskUV *task1, TaskUV *task2)
	:_offset(0)
{
	_task[0] = task1;
	_task[1] = task2;
}

void BiTask::Proceed()
{
	//TODO
	delete this;
}
