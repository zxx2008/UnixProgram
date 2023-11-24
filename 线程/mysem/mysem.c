#include "mysem.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

//信号量结构体
struct mysem_st
{
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

mysem_t* mysem_init(int initVal) {
    struct mysem_st* p = malloc(sizeof(*p));
    if (p == NULL) exit(1);
    p->count = initVal;
    //初始化互斥量和条件变量
    pthread_mutex_init(&p->mutex, NULL);
    pthread_cond_init(&p->cond, NULL);
    return p;
}

int mysem_add(mysem_t* p, int num) {
    //锁！！！
    struct mysem_st* ptr = p;
    pthread_mutex_lock(&ptr->mutex);
    ptr->count += num;
    //发信号唤醒阻塞
    pthread_cond_broadcast(&ptr->cond);
    //解锁
    pthread_mutex_unlock(&ptr->mutex);
    return num;
}

int mysem_sub(mysem_t* p, int num) {
    struct mysem_st* ptr = p;
    //锁
    pthread_mutex_lock(&ptr->mutex);
    //count < num阻塞
    while (ptr->count < num) {
        pthread_cond_wait(&ptr->cond, &ptr->mutex);
    }
    ptr->count -= num;
    //解锁
    pthread_mutex_unlock(&ptr->mutex);
    return num;
}

int mysem_destory(mysem_t* p) {
    struct mysem_st* ptr = p;
    pthread_mutex_destroy(&ptr->mutex);
    pthread_cond_destroy(&ptr->cond);
    free(ptr);
    return 0;
}
