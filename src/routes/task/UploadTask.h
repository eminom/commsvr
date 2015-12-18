


#ifndef _UPLOAD_TASK_DEF__
#define _UPLOAD_TASK_DEF__

#include "haywire.h"
#include "TaskUVBase.h"

#include <string>
#include "uv.h"

#define _UtBody(name)\
	void on_##name(uv_fs_t*);\
	static void ut_##name(uv_fs_t *req){\
		uv_fs_req_cleanup(req);\
		UploadTask *self = (UploadTask*)req->data;\
		self->on_##name(req);\
	}

class UploadTask:public TaskUV {
	//typedef UploadTask this_class;
public:
	virtual ~UploadTask();

public:
	UploadTask(hw_string*, const std::string &full);
	virtual void Proceed()override;

private:
	_UtBody(fileOpen)
	_UtBody(fileWrite)
	_UtBody(fileClose)

	void goToOpen();
	void goToWrite();
	void goToClose();

private:
	hw_string *_body;
	int _written;	//~ no more that 32G(SURE)
	int _left;
	std::string _finalpath;

	uv_fs_t open_req;
	uv_fs_t write_req;
	uv_fs_t close_req;
	uv_buf_t write_buf;

private:
	UploadTask(const UploadTask&);
	void operator=(const UploadTask&);
};

#endif