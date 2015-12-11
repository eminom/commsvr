

#include "TestDep.h"
#include "uv.h"
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>

void onTestClose(uv_handle_t *handle)
{
	printf("Void() = %p\n", handle);
}

int TestEntry()
{
	uv_write_t write_req;
	memset(&write_req, 0, sizeof(write_req));
	//uv_req_init(uv_default_loop(), &write_req);
	uv_close((uv_handle_t*)(write_req.handle), onTestClose);

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	printf("The end.\n");
	return 0;
}