

#include "DeleteTask.h"
#include "uv.h"

DeleteTask::~DeleteTask(){
}

DeleteTask::DeleteTask(const std::string &target, const std::string &refUrl)
	:_target(target)
	,_refUrl(refUrl)
{
}

void DeleteTask::Proceed(){
	goToDelete();
}


void DeleteTask::goToDelete(){
	del_req.data = this;
	uv_fs_unlink(uv_loop, &del_req, _target.c_str(), ut_deleteFile);
}

void DeleteTask::on_deleteFile(uv_fs_t *req){
	if(req->result){
		fprintf(stderr, "error delete:<%s> :%s\n",
			_refUrl.c_str(),
			uv_strerror(req->result)
			);
	}
	delete this;
}