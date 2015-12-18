

#include "services/Inception.h"
#include "haywire.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include "files/RootExplorer.h"
#include "utils/SlashDef.h"

//#define CRLF "\r\n"
#include "base/HtmlContentType.h"
#include "utils/ResponseUtils.h"
#include "config/HttpServerConfig.h"
#include "files/DirMaker.h"

#include "data/UploadTask.h"
#include <uv.h>


extern "C" uv_loop_t *uv_loop; // Defined in haywire http server module.

#define _END_IN_MAL()\
	{\
		finish_response(request\
			, response\
			, HTTP_STATUS_500\
			, (void*)"file upload request done"\
			, ContentType_TextPlain\
			, "Malicious Request"\
			);\
		return;\
	}

void on_work_start(uv_work_t *req){
	UploadTask *uTask = (UploadTask*)req->data;
	uTask->Proceed();
}

void on_work_done(uv_work_t *req, int status){
	//UploadTask *uTask = (UploadTask*)req->data;
	//delete uTask;
	free(req);
}

// request->body->length shall be strictly equal to file size(no matter what form it is)
void get_upload(http_request *request, hw_http_response *response, void *user_data) {

	const char *prep = "name=";
	const char *beg = strchr(request->url, '/');
	beg = (beg?beg:request->url);
	beg = strchr(beg, '?');
	std::string path;
	if (beg && strlen(beg) > 1 + strlen(prep) && !strncmp(prep, beg+1, strlen(prep)))
		path = beg + 1 + strlen(prep);
	if(path.empty())
		_END_IN_MAL()

	//printf("store in <%s>\n", path.c_str());
	const char *pre = strrchr(path.c_str(), '/');
	std::string sub = pre ? std::string(path.c_str(), pre - path.c_str()) : "";
	std::string finalpath = RootExplorer::getInstance()->getWorkingDir() + path;
	std::string tpath = RootExplorer::getInstance()->getWorkingDir() + sub;

	MkdirRecur(tpath);//~ TODO: in async mode?(or do it in this hacking way)

	//_AllocObj()
	//_DefsObj(ptr)
	//*ppRequest = request;
	//*ppResponse= response;
	//*pszWritePath = new std::string(finalpath);
	//*pszDirPath   = new std::string(pszDirPath);
	//slashpos    = 0;

	uv_work_t *work_req = (uv_work_t*)malloc(sizeof(uv_work_t));
	UploadTask *uTask = new UploadTask(request->body, finalpath);
	work_req->data = uTask;
	uv_queue_work(uv_loop, work_req, on_work_start, on_work_done);

	//: replace the old one. 
	request->body = (hw_string*)malloc(sizeof(hw_string));
	request->body->length = 0;
	request->body->value = nullptr;
	//const char *resCode = HTTP_STATUS_500;
	const char *resCode = HTTP_STATUS_200;
	finish_response(request,
		response,
		resCode,
		(void*)("Uploading"),
		ContentType_TextPlain,
		"Upload complete"
		);
}