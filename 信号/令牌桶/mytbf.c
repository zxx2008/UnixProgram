#include <stdio.h>
#include <stdlib.h>

#include "mytbf.h"

/// @brief 令牌桶结构体
struct mytbf_st {
    int csp;    //令牌桶传输速度 csp字符/秒
    int burst;  //令牌桶令牌数量上限
    int token;  //令牌桶令牌数量
    int pos;    //任务列表下标
};

static struct mytbf_st job[MYTBF_MAX];

