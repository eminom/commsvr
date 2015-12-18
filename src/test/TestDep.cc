

#include "TestDep.h"
#include "uv.h"
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>
#include <uv.h>

#ifdef _MSC_VER
#include <Windows.h>
#define sleep(a)	::Sleep(a*1000)
#define random()	rand()
#endif

#define FIB_UNTIL 100
//extern uv_loop_t *loop;

long fib_(long t) {
    if (t == 0 || t == 1)
        return 1;
    else
        return fib_(t-1) + fib_(t-2);
}

void fib(uv_work_t *req) {
    int n = (int) req->data;
	printf("Starting calc fib<%d>\n", n);
    //if (random() % 2)
    //    sleep(1);
    //else
    //    sleep(3);
    long fib = fib_(n);
	printf("Done calc fib<%d>\n", n);
	//free(req); // Corrupted(some lock may still remain in position)
    //fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}

void after_fib(uv_work_t *req, int status) {
    //fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *) req->data);
	free(req);
}

int TestEntry() {
    uv_loop_t *loop = uv_default_loop();
    for (int i = 0; i < FIB_UNTIL; i++) {
		uv_work_t *req = (uv_work_t*)malloc(sizeof(uv_work_t));
		req->data = (int*)i;
        uv_queue_work(loop, req, fib, after_fib);
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}
