/*使用锁和条件变量完成在终端不断输出abcdabcdabcd直到alarm信号到来*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THRNUM 4

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//全局变量控制输出的顺序，初始值为0表示先输出'a'
static int num = 0;

//实现n + 1 mod THRNUM
static int next(int n) {
    return (n + 1) == THRNUM ? 0 : n + 1;
}

//定义信号处理函数

static void* thr_func(void* p) {
    //每个线程获取自己的输出顺序
    int n = (int)p;
    //计算本线程需要输出的值
    char c = 'a' + n;
    //循环输出
    while (1) {
        //有输出不管三七二十一，先锁
        pthread_mutex_lock(&mutex);
        //没到 自己的回合阻塞
        while (n != num) {
            // 解锁等待！！！！！， 唤醒后抢锁继续执行，抢不到继续等待！！！！
            pthread_cond_wait(&cond, &mutex);
        }
        write(1, &c, 1);
        //num++
        num = next(num);
        //唤醒其他阻塞
        //解锁
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    int err;
    //创建THRNUM个线程
    pthread_t ptid[THRNUM];
    for (int i = 0; i < THRNUM; ++i) {
        err = pthread_create(ptid+i, NULL, thr_func, (void*)i);
        if (err) {
            fprintf(stderr, "pthread_create is error %d", err);
        }
    }
    
    alarm(3);

    //join
    for (int i = 0; i < THRNUM; ++i) {
        pthread_join(ptid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    exit(0);
}