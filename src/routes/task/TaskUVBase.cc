

#include "TaskUVBase.h"

namespace taskUV {

void on_work_start(uv_work_t *req){
	TaskUV *uTask = reinterpret_cast<TaskUV*>(req->data);
	uTask->Proceed();
}

void on_work_done(uv_work_t *req, int status){
	free(req);
}



}