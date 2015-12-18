

#include "UploadTask.h"
#include <fcntl.h>
#include <functional>
#include <algorithm>

extern "C" uv_loop_t *uv_loop;

#define _SEGMENT_SIZE	65536

UploadTask::~UploadTask()
{
	if(_body){
		if(_body->value){
			free(_body->value);
		}
		free(_body);
	}
}

UploadTask::UploadTask(hw_string *body, const std::string &finalpath)
	:_body(body)
	,_finalpath(finalpath)
	,_written(0)
	,_left(body->length)
{
	
}

void UploadTask::Proceed(){
	goToOpen();
}

void UploadTask::goToOpen(){
	open_req.data = this;
	uv_fs_open(uv_loop, &open_req, _finalpath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666, ut_fileOpen);
}

void UploadTask::goToWrite()
{
	write_req.data = this;
	int toWrite = _left < _SEGMENT_SIZE ? _left : _SEGMENT_SIZE;
	write_buf = uv_buf_init(_body->value + _written, toWrite);
	uv_fs_write(uv_loop, &write_req, open_req.result, &write_buf, 1, -1, ut_fileWrite);
}

void UploadTask::goToClose() {
	close_req.data = this;
	uv_fs_close(uv_loop, &close_req, open_req.result, ut_fileClose);
}

void UploadTask::on_fileOpen(uv_fs_t *req) {
	if (req->result >= 0) {
		goToWrite();
	} else {
		fprintf(stderr, "error opening:<%s> :%s\n", _finalpath.c_str(), uv_strerror(req->result));
		delete this;
	}
}

void UploadTask::on_fileWrite(uv_fs_t *req) {
	if (req->result >= 0) {
		_written += req->result;
		_left -= req->result;
		if(_left <= 0){
			goToClose();
		} else {
			goToWrite();
		}
	} else {
		fprintf(stderr, "error writing:<%s> :%s\n", _finalpath.c_str(), uv_strerror(req->result));
		goToClose();
	}
}

void UploadTask::on_fileClose(uv_fs_t *req) {
	if(req->result){
		fprintf(stderr, "error closing:<%s> :%s\n", _finalpath.c_str(), uv_strerror(req->result));
	}
	delete this;
}

/*
void UploadTask::Proceed()
{
	if(FILE *fout = fopen(_finalpath.c_str(), "wb")){
		if(_body && _body->value){
			auto written = fwrite(_body->value, 1, _body->length, fout);
			if(written == _body->length){
				//responseCode = HTTP_STATUS_200;
			}
		}
		fclose(fout);
	}
}
*/



