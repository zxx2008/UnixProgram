/*使用互斥量和条件变量实现信号量*/
#ifndef _MYSEM_H_
#define _MYSEM_H_

typedef void mysem_t;

//初始化信号量
mysem_t* mysem_init(int initVal);
//信号量-num
int mysem_sub(mysem_t* p, int num);
//信号量+num
int mysem_add(mysem_t* p, int num);
//释放内存
int mysem_destory(mysem_t* p);

#endif