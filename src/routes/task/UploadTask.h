


#ifndef _UPLOAD_TASK_DEF__
#define _UPLOAD_TASK_DEF__

#include "haywire.h"
#include "TaskUVBase.h"

#include <string>
#include "uv.h"


class UploadTask:public TaskUV {
	//typedef UploadTask this_class;
public:
	virtual ~UploadTask();

public:
	UploadTask(hw_string*, const std::string &full);
	UploadTask(hw_string*, const std::string &full, const std::string &x2hash, unsigned int seed);
	virtual void Proceed()override;

private:
	_UtBody(fileOpen, UploadTask)
	_UtBody(fileWrite,UploadTask)
	_UtBody(fileClose,UploadTask)

	void goToOpen();
	void goToWrite();
	void goToClose();

private:
	hw_string *_body;
	int _written;	//~ no more that 32G(SURE)
	int _left;
	std::string _finalpath;
	std::string _xxhash;
	unsigned int _seed;

	uv_fs_t open_req;
	uv_fs_t write_req;
	uv_fs_t close_req;
	uv_buf_t write_buf;

private:
	UploadTask(const UploadTask&);
	void operator=(const UploadTask&);
};

#endif