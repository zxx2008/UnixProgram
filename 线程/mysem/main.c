#include <stdio.h>
#include <stdlib.h>
#include "mysem.h"
#include <pthread.h>

#define LEFT  30000000
#define RIGHT 30000200
#define THRNUM (RIGHT-LEFT+1)
#define N 4     //定义最大的共享量

//定义一个信号量为全局变量
static mysem_t* sem;

static void * thr_func(void* p) {
    int i, j, mark;
    mark = 1;
    i = (int)p;
    for (j = 2; j < i / 2; ++j) {
        if ((i % j) == 0) {
            mark = 0;
            break;
        }
    }
    
    if (mark) {
        printf("%d is a primer\n", i);
    }

    //线程结束释放一个信号量
    mysem_add(sem, 1);
    pthread_exit(NULL);
}

int main() {
    int err;
    pthread_t ptid[THRNUM];
    //初始化信号量，最大共享数量为4
    sem = mysem_init(N);
    //创建THRNUM个线程完成任务，但最大的线程数量为4
    for (int i = LEFT; i <= RIGHT; ++i) {
        //判断信号量，阻塞
        mysem_sub(sem, 1);
        //创建线程
        err = pthread_create(ptid+(i-LEFT), NULL, thr_func, (void*)i);
        if (err) {
            fprintf(stderr, "pthread_create is error:%d\n", err);
        }
    }
    
    for (int i = LEFT; i <= RIGHT; ++i) {
        pthread_join(ptid[i-LEFT], NULL);
    }

    mysem_destory(sem);
    exit(0);
}