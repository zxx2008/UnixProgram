/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-24 17:49:48
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-24 20:06:13
 * @FilePath: /令牌桶/mytbf.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include "mytbf.h"

/// @brief 令牌桶结构体
struct mytbf_st {
    int cps;    //令牌桶传输速度 csp字符/秒
    int burst;  //令牌桶令牌数量上限
    int token;  //令牌桶令牌数量
    int pos;    //任务列表下标
};

static struct mytbf_st* job[MYTBF_MAX];
static int inited = 0;
static __sighandler_t alarm_old_handler;

/// @brief 获取令牌桶指针数组中未初始化的一项
/// @return 获取成功返回索引，失败返回-1
int get_free_pos() {
    for (int i = 0; i < MYTBF_MAX; ++i) {
        if (job[i] == NULL) {
            return i;
        }
    }
    return -1;
}

/// @brief alarm信号处理函数
/// @param s 
void alarm_handler(int s) {
    alarm(1);
    for (int i = 0; i < MYTBF_MAX; ++i) {
        if (job[i] != NULL) {
            mytbf_return_token(job[i], job[i]->cps);
        }
    }
}


static void mod_unload() {
    signal(SIGALRM, alarm_old_handler);
    alarm(0);
    for (int i = 0; i < MYTBF_MAX; ++i) {
        free(job[i]);
        job[i] = NULL;
    }
}

/// @brief alarm信号流控初始化
static void mod_load() {
    alarm_old_handler = signal(SIGALRM, alarm_handler);
    alarm(1);
    atexit(mod_unload);
}


mytbf_t* mytbf_init(int cps, int burst) {
    
    struct mytbf_st* obj;

    //初始化时钟信号
    if(!inited) {
        // 只执行一次
        mod_load();
    }
    
    int pos = get_free_pos();
    if (pos < 0) {
        return NULL;
    }
    obj = malloc(sizeof(struct mytbf_st));
    if (obj == NULL) {
        return NULL;
    }
    obj->cps = cps;
    obj->burst = burst; 
    obj->pos = pos;
    obj->token = 0;
    job[pos] = obj;

    return obj;
}

int mytbf_fetch_token(mytbf_t* obj, int num) {

    //类型转换
    struct mytbf_st* ptr = obj;

    if (num <= 0) {
        return -EINVAL;
    }

    while (ptr->token <= 0) {
        pause();
    }

    int n = ptr->token < num ? ptr->token : num;
    ptr->token -= n;
    return n;
}

int mytbf_return_token(mytbf_t* obj, int num) {
    
    if (num <= 0) {
        return -EINVAL;
    }

    //类型转换
    struct mytbf_st* ptr = obj;

    int n;
    if (ptr->token + num >= ptr->burst) {
        n = ptr->burst - num;
        ptr->token = ptr->burst;
    }
    else {
        ptr->token += num;
        n = num;
    }

    return n;
}

int mytbf_destory(mytbf_t* obj) {
    struct mytbf_st * ptr = obj;
    job[ptr->pos] = NULL;
    free(ptr);
    return 0;    
}


