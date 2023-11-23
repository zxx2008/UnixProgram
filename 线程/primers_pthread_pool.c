/*创建若干个线程，竞争完成对素数的检查*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THRNUM 4
#define LEFT 30000000
#define RIGHT 30000200

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int num = 0;

static void* handler(void* p) {
    // int task, mark;
    int task;
    int mark;
    // 循环
    while (1)
    {
        mark = 1;
        //处理任务，锁！
        pthread_mutex_lock(&mutex);
        //num == 0 等待
        while (num == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        //num == -1 退出
        if (num == -1) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        // 领取任务task = num;
        task = num;
        // num置为0
        num = 0;
        // 发出信号
        pthread_cond_broadcast(&cond);
        // 解锁
        pthread_mutex_unlock(&mutex);

        //处理任务，判断是否为素数
        for (int n = 2; n < task / 2; ++n) {
            if (task % n == 0) {
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("[thread-%d] %d is a primer\n", (int)p, task);
        }
    }
    pthread_exit(NULL);
    
}

int main() {
    //定义线程数组
    pthread_t ptid[THRNUM];

    //创建线程

    int err;
    for (int i = 0; i < THRNUM; ++i) {
        err = pthread_create(ptid+i, NULL, handler, (void*)i);
        if (err) {
            fprintf(stderr, "pthread_create is error: %d\n", err);
            exit(1);
        }
    }

    //从LEFT开始循环到RIGHT，分配任务
    for (int i = LEFT; i <= RIGHT; ++i) {
        //分配任务前加锁
        pthread_mutex_lock(&mutex);
        //任务没有被领取等待
        while (num != 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        
        //任务成功下发，发信号
        num = i;
        pthread_cond_signal(&cond);
        //解锁
        pthread_mutex_unlock(&mutex);
    }
    //万一任务没有被执行完，判断一下，先加锁
    pthread_mutex_lock(&mutex);
    //判断任务是否被执行完，num == 0 ?
    while (num != 0) {
        //阻塞等待执行完成
        pthread_cond_wait(&cond, &mutex);
    }
    //num 置为 -1
    num = -1;
    //通知线程任务完成
    pthread_cond_broadcast(&cond);
    //解锁
    pthread_mutex_unlock(&mutex);
    //join回收线程

    for (int i = 0; i < THRNUM; ++i) {
        pthread_join(ptid[i],NULL);
    }

    //destory mutex 和 cond
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    exit(0);
}