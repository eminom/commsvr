

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


// request->body->length shall be strictly equal to file size(no matter what form it is)
void get_upload(http_request *request, hw_http_response *response, void *user_data){

	const char *prep = "name=";
	const char *beg = strchr(request->url, '/');
	beg = (beg?beg:request->url);
	beg = strchr(beg, '?');
	std::string path;
	if (beg && strlen(beg) > 1 + strlen(prep) && !strncmp(prep, beg+1, strlen(prep)))
		path = beg + 1 + strlen(prep);
	if(path.empty())
		_END_IN_MAL()

	printf("store in <%s>\n", path.c_str());
	const char *pre = strrchr(path.c_str(), '/');
	std::string sub = pre ? std::string(path.c_str(), pre - path.c_str()) : "";
	std::string finalpath = RootExplorer::getInstance()->getWorkingDir() + path;
	std::string tpath = RootExplorer::getInstance()->getWorkingDir() + sub;

	MkdirRecur(tpath);

	//_AllocObj()
	//_DefsObj(ptr)
	//*ppRequest = request;
	//*ppResponse= response;
	//*pszWritePath = new std::string(finalpath);
	//*pszDirPath   = new std::string(pszDirPath);
	//slashpos    = 0;

	const char *resCode = HTTP_STATUS_500;
	if(FILE *fout = fopen(finalpath.c_str(), "wb"))	{
		if(request->body && request->body->value){
			auto written = fwrite(request->body->value, 1, request->body->length, fout);
			if(written == request->body->length)
				resCode = HTTP_STATUS_200;
		}
		fclose(fout);
	}
	finish_response(request,
		response,
		resCode,
		(void*)("Uploading"),
		ContentType_TextPlain,
		"Upload complete"
		);
}