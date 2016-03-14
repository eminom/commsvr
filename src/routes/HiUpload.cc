

#include "services/Inception.h"
#include "haywire.h"

#include <string>
#include <map>
#include <cstring>
#include <cstdlib>
#include "files/RootExplorer.h"
#include "utils/SlashDef.h"

//#define CRLF "\r\n"
#include "base/HtmlContentType.h"
#include "utils/ResponseUtils.h"
#include "config/HttpServerConfig.h"
#include "files/DirMaker.h"

#include "task/UploadTask.h"
#include <uv.h>


extern "C" uv_loop_t *uv_loop; // Defined in haywire http server module.

void SplitToDict(const char *url, std::map<std::string, std::string> &dict) {
	const char *ed = url + strlen(url);
	for(const char *ptr=url; ptr != ed;){
		const char *sp = strchr(ptr, '&');
		sp = sp?sp:ed;
		const char *eq = strchr(ptr, '=');
		eq = eq?eq:ed;
		if(eq >= sp){
			break;
		}
		if( eq - ptr > 0 && sp - (eq+1) > 0) {
			std::string key(ptr, eq - ptr);
			std::string val(eq+1, sp - (eq+1));
			dict[key] = val;
		}
		if(sp == ed)
			break;
		ptr = sp + 1;
	}
}

// request->body->length shall be strictly equal to file size(no matter what form it is)
void get_upload(http_request *request, hw_http_response *response, void *user_data) {
	const char *beg = strchr(request->url, '/');
	beg = (beg?beg:request->url);
	beg = strchr(beg, '?');
	std::string params;
	if (beg)
		params = beg + 1;
	std::map<std::string, std::string> dc;
	SplitToDict(params.c_str(), dc);
	auto name_pos = dc.find("name");
	if(params.empty() || dc.end() == name_pos) {
		finish_response(request
			, response
			, HTTP_STATUS_500
			, _BuildRStr("Invalid upload url format")
			, ContentType_TextPlain
			, "Malicious Request"
			);
		return;
	}
	std::string lpath = name_pos->second; //Copy name
	std::string hash;
	auto hash_pos = dc.find("hash");
	if (hash_pos != dc.end()) {
		hash = hash_pos->second;
	}

	//printf("store in <%s>\n", lpath.c_str());
	const char *pre = strrchr(lpath.c_str(), '/');
	std::string sub = pre ? std::string(lpath.c_str(), pre - lpath.c_str()) : "";
	std::string finalpath = RootExplorer::getInstance()->getWorkingDir() + lpath;
	std::string tpath = RootExplorer::getInstance()->getWorkingDir() + sub;

	MkdirRecur(tpath);//~ TODO: in async mode?(or do it in this hacking way)

	//_AllocObj()
	//_DefsObj(ptr)
	//*ppRequest = request;
	//*ppResponse= response;
	//*pszWritePath = new std::string(finalpath);
	//*pszDirPath   = new std::string(pszDirPath);
	//slashpos    = 0;
	if(hash.size()>0){
		queueSimpleTaskUV(new UploadTask(request->body, finalpath, hash, 10241024));
	} else {
		queueSimpleTaskUV(new UploadTask(request->body, finalpath));
	}

	//: replace the old one. 
	request->body = (hw_string*)malloc(sizeof(hw_string));
	request->body->length = 0;
	request->body->value = nullptr;
	//const char *resCode = HTTP_STATUS_500;
	const char *resCode = HTTP_STATUS_200;
	finish_response(request,
		response,
		resCode,
		_BuildRStr(std::string("Upload for <") + request->url + ">"),
		ContentType_TextPlain,
		"Upload complete"
		);
}