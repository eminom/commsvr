
#define _AllocObj() \
	int _cb = sizeof(uv_fs_t) * 4 \
		+ sizeof(http_request*)   \
		+ sizeof(hw_http_response*)  \
		+ sizeof(std::string*)    \
		+ sizeof(std::string*)    \
		+ sizeof(hw_string*)      \
		+ sizeof(int)			  \
		+ sizeof(uv_buf_t);
	void *ptr = malloc(_cb);\
	memset(ptr, 0, _cb);\

#define _DefsObj(_PTR)\
	uv_fs_t *mkdir_req= (uv_fs_t*)(_PTR);\
	uv_fs_t *open_req = (uv_fs_t*)(mkdir_req+1);\
	uv_fs_t *write_req= (uv_fs_t*)(open_req+1);\
	uv_fs_t *close_req= (uv_fs_t*)(write_req+1);\
	http_request **ppRequest  = (http_request**)(close_req+1);\
	hw_http_response **ppResponse= (hw_http_response**)(ppRequest+1);\
	std::string **pszWritePath = (std::string**)(ppResponse + 1);\
	std::string **pszDirPath   = (std::string**)(pszWritePath + 1);\
	int& slashpos = *((int*)(pszDirPath+1));\
	uv_buf_t *write_buf = (uv_buf_t*)(&slashpos+1);

void on_file_open(uv_fs_t *req);
void on_file_write(uv_fs_t *req);
void on_file_close(uv_fs_t *req);
void on_dir_make(uv_fs_t *req);

void startWriteFile(void *ptr)
{
	_DefsObj(ptr);
	uv_fs_open(uv_loop, open_req, (*pszWritePath)->c_str(), O_WRONLY, 0, on_file_open);
}

void startMkdir(void *ptr)
{
	_DefsObj(ptr)
	while (slashpos < (*pszDirPath)->size()) {
		char buf[BUFSIZ];
		while (1) {
			const char *lpsz = (*pszDirPath)->c_str() + slashpos;
			const char *lp = strchr(lpsz, '/');
			if(lp){
				if(0==lp-lpsz){
					++slashpos;
					continue;
				}
				int slen = lp - lpsz;
				strncpy(buf, lpsz, slen);
				buf[slen] = 0;
				uv_fs_mkdir(uv_loop, mkdir_req, buf, 0777, on_dir_make);
				return;
			}
			if(strlen(lpsz) > 0){
				uv_fs_mkdir(uv_loop, mkdir_req, lpsz, 0777, on_dir_make);
				return;
			}
		}
	}
	startWriteFile(ptr);
}

void on_dir_make(uv_fs_t *req) {
	uv_fs_req_cleanup(req);
	void *ptr = (void*)req;
	_DefsObj(ptr)
	//Don't care about the mkdir result.
	if(req->result < 0) {
		startWriteFile(ptr);
		return;
	}
	startMkdir(ptr);
}

void on_file_open(uv_fs_t *req){
	uv_fs_req_cleanup(req);
	void *ptr = (void*)(req-1);
	_DefsObj(ptr)
	if(req->result>=0){
		write_req->data = ptr;



}