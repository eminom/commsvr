


#ifndef _MY_UV_TASK_TASK_UV_MY__DEF__
#define _MY_UV_TASK_TASK_UV_MY__DEF__

#include "uv.h"
#include <cstdlib>

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

#define _UtBody(name, TYPE)\
	void on_##name(uv_fs_t*);\
	static void ut_##name(uv_fs_t *req){\
		uv_fs_req_cleanup(req);\
		TYPE *self = (TYPE*)req->data;\
		self->on_##name(req);\
	}

#endif