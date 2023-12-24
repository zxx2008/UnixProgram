//令牌桶的封装

#ifndef _MYTBF_H_
#define _MYTBF_H_

#define MYTBF_MAX 1024  //令牌桶数量上限

typedef void mytbf_t;

/// @brief 初始化令牌桶
/// @param cps 令牌桶传输速度
/// @param burst 令牌数量上限
/// @return 指向令牌桶的指针,初始化失败返回NULL
mytbf_t* mytbf_init(int cps, int burst);

/// @brief 删除令牌桶
/// @param obj 指向令牌桶的指针
/// @return 正确返回0,错误返回负值
int mytbf_destory(mytbf_t* obj);

/// @brief 获取token
/// @param obj 
/// @param num 获取token数量
/// @return 正确返回获取token数量,错误返回负值
int mytbf_fetch_token(mytbf_t* obj, int num);

/// @brief 归还token
/// @param obj 
/// @param num 归还令牌数量 
/// @return 正确返回归还token数量,错误返回负值
int mytbf_return_token(mytbf_t* obj, int num);

#endif