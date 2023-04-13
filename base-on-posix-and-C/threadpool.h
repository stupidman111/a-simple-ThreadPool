#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <pthread.h>
#define FALSE 0
#define TRUE !0
#define DEFAULT_TIME 1
#define MIN_WAIT_TASK_NUM 5
#define DEFAULT_THREAD_NUM 4
typedef struct {
    void* (*function) (void*);
    void* arg;
} threadpool_task_t;

/* 线程池管理 */
struct threadpool_t {
    pthread_mutex_t lock;   /* 互斥量-对除忙线程以外的锁 */
    pthread_mutex_t thread_counter; /* 用于使用忙线程数时的锁 */
    pthread_cond_t queue_not_full; /* 条件变量-任务队列不满 */
    pthread_cond_t queue_not_empty; /* 条件变量-任务队列不空*/

    pthread_t* threads; /* 存放线程的tid，实际即使管理线程的数组 */
    pthread_t admin_tid; /* 管理者线程tid */
    threadpool_task_t* task_queue; /* 任务队列 */

    /* 线程池信息 */
    int min_thr_num; /* 线程池中最小线程数 */
    int max_thr_num; /* 线程池中最大线程数 */
    int live_thr_num; /* 线程池中存活的线程数 */
    int busy_thr_num; /* 忙线程，正在工作的线程 */
    int wait_exit_thr_num; /* 需要销毁的线程数 */

    /* 任务队列信息 */
    int queue_front; /* 队头 */
    int queue_rear; /* 队尾 */
    int queue_size; 

    int queue_max_size; /* 最大任务数-任务队列最大能容纳的任务数 */
    int shutdown; /* 线程池状态-true为关闭 */
};

typedef struct threadpool_t threadpool_t;

/* 创建线程池 */
threadpool_t* threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);
                /* 最小线程数       最大线程数          最大任务数  */

/* 工作线程 */
void* threadpool_thread(void* threadpool);

/* 向线程池的任务队列中添加一个任务 */
int threadpool_add_task(threadpool_t* pool, void* (*function)(void* arg), void* arg);

/* 管理者线程 */
void* admin_thread(void* threadpool);

/* 释放线程池 */
int threadpool_free(threadpool_t* pool);

/*销毁线程池*/
int threadpool_destroy(threadpool_t *pool);

#endif

