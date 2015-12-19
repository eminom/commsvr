

#ifndef _DELTE_TASK_DEF__HIHIHI_
#define _DELTE_TASK_DEF__HIHIHI_

#include "TaskUVBase.h"
#include <string>



class DeleteTask:public TaskUV{
public:
	virtual ~DeleteTask();

public:
	DeleteTask(const std::string &file_path, const std::string &refUrl);
	virtual void Proceed()override;

private:
	_UtBody(deleteFile, DeleteTask)

private:
	void goToDelete();

private:
	const std::string _target;
	const std::string _refUrl;
	uv_fs_t del_req;

private:
	DeleteTask(const DeleteTask&);
	void operator=(const DeleteTask&);
};

#endif