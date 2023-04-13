#include <stdio.h>
#include "threadpool.h"

void* do_work(void* arg) {
    //threadpool_t* pool = (threadpool_t*) arg;
    printf("++++");
}

int main() {
    /* 线程池初始化，其管理者线程及工作线程都会启动 */
    threadpool_t *thp = threadpool_create(10, 100, 100);
    printf("threadpool init ... ... \n");

    /* 接收到任务后添加 */
    threadpool_add_task(thp, do_work, (void *)thp);

    // ... ...

    /* 销毁 */
    threadpool_destroy(thp);

    return 0;
}