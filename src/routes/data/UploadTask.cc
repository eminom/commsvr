

#include "UploadTask.h"

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
{

}

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


