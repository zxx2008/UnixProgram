#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "mytbf.h"

//取a和b的最小值
static int min(int a, int b) {
    return (a > b ? b : a);
}

// 令牌桶结构体
struct mytbf_st
{
    int cps;    //速度
    int burst;  //令牌上限
    int token;  //令牌数
    int pos;    // 位置
    pthread_mutex_t mut;    //互斥量
    pthread_cond_t cond;    //条件变量
};

static struct mytbf_st *job[MYTBF_MAX]; //全局变量定义令牌桶指针数组

static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER; //全局互斥量用于保证令牌桶数组的操作安全

static pthread_once_t init_once = PTHREAD_ONCE_INIT;    //多线程只执行一次

static pthread_t tid; //时钟线程

/// @brief 寻找job空位
/// @param  
/// @return 空位索引, 失败返回-1
static int get_free_pos_unlocked(void) {
    int i;
    for(i = 0; i < MYTBF_MAX; ++i) {
        if(job[i] == NULL) {
            return i;
        }
    }
    return -1;
}

// 时钟函数
static void * thr_alrm(void *p) {
    //
     while(1) {
        pthread_mutex_lock(&mut_job);
        for(int i = 0; i < MYTBF_MAX; ++i) {
            if(job != NULL) {
                pthread_mutex_lock(&job[i]->mut);
                job[i]->token += job[i]->cps;
                if (job[i]->token > job[i]->burst) {
                    job[i]->token = job[i]->burst;
                }
                pthread_cond_broadcast(&job[i]->cond);
                pthread_mutex_unlock(&job[i]->mut);
            }
        }
        pthread_mutex_unlock(&mut_job);
        sleep(1);
     }
}


// 时钟模块卸载
static void module_unload(void) {
    int i;
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    for(i = 0; i < MYTBF_MAX; ++i) {
        free(job[i]);
    }
    return ;
}

// 时钟模块加载
static void module_load(void) {
    
    int err;

    err = pthread_create(&tid, NULL, thr_alrm, NULL);
    if(err) {
        fprintf(stderr, "pthread_create(): %s\n", strerror(errno));
        exit(1);
    }
    
    // 模块卸载挂在钩子函数上
    atexit(module_unload);
}

mytbf_t *mytbf_init(int cps, int burst) {
    struct mytbf_st *me;
    pthread_once(&init_once, module_load);
    me = malloc(sizeof(*me));
    if(me == NULL) {
        return NULL;
    }

    me->cps = cps;
    me->burst = burst;
    me->token = 0;
    pthread_mutex_init(&me->mut, NULL);
    pthread_cond_init(&me->cond, NULL);

    // 加锁
    pthread_mutex_lock(&mut_job);
    int pos = get_free_pos_unlocked();
    if(pos < 0) {
        pthread_mutex_unlock(&mut_job);
        free(me);
        return NULL;
    }

    me->pos = pos;
    job[me->pos] = me;
    pthread_mutex_unlock(&mut_job);
    return me;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size) {
    struct mytbf_st *me = ptr;
    // 结构体中的互斥量负责当前结构体操作的线程安全
    pthread_mutex_lock(&me->mut);
    // 利用条件变量等待token
    while(me->token <= 0) {
        pthread_cond_wait(&me->cond, &me->mut);
    }
    int n = min(me->token, size);
    me->token -= n;
    pthread_mutex_unlock(&me->mut);
}

int mytbf_returntoken(mytbf_t *ptr, int size) {
    struct mytbf_st *me = ptr;
    pthread_mutex_lock(&me->mut);
    int n = me->token;
    me->token += size;
    if(me->token > me->burst) {
        me->token = me->burst;
    }
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);
    return (me->token - n);
}

int mytbf_destroy(mytbf_t *ptr) {
    struct mytbf_st *me = ptr;

    pthread_mutex_lock(&mut_job);
    job[me->pos] = NULL;
    pthread_mutex_unlock(&mut_job);

    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(ptr);
    return 0;
}

int mytbf_checktoken(mytbf_t *ptr) {
    int token_left = 0;
    struct mytbf_st *me = ptr;
    pthread_mutex_lock(&me->mut);
    token_left = me->token;
    pthread_mutex_unlock(&me->mut);
    return token_left;
}