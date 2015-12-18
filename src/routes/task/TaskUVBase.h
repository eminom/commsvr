


#ifndef _MY_UV_TASK_TASK_UV_MY__DEF__
#define _MY_UV_TASK_TASK_UV_MY__DEF__

#include "uv.h"

extern "C" uv_loop_t *uv_loop; // Implemented in haywire http server.

namespace taskUV {
	void on_work_start(uv_work_t *);
	void on_work_done(uv_work_t *, int);
}

class TaskUV {
public:
	virtual void Proceed() = 0;
};

inline void queueSimpleTaskUV(TaskUV *task) {
	uv_work_t *__req = (uv_work_t*)malloc(sizeof(uv_work_t));
	__req->data = task;
	uv_queue_work(uv_loop, __req, &taskUV::on_work_start, &taskUV::on_work_done);
}


#endif