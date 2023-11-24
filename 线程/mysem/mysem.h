/*使用互斥量和条件变量实现信号量*/
#ifndef _MYSEM_H_
#define _MYSEM_H_

typedef void mysem_t;

//初始化信号量
mysem_t* mysem_init(int );
//信号量-num
int mysem_sub(mysem_t* , int );
//信号量+num
int mysem_add(mysem_t* , int );
//释放内存
int mysem_destory(mysem_t*);

#endif