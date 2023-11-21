#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM (RIGHT - LEFT + 1)

struct num {
    int n;
};

static void* thr_primer(void* p) {
    //判断是否是素数的标记位
    int mark = 1;
    //获得需要判断的整型
    int i = ((struct num*)p)->n; 
    //判断是否为素数
    for (int j = 2; j < i/2; ++j) {
        if (i % j == 0) {
            mark = 0;
            break;
        }
    }

    if(mark == 1) {
        printf("%d is a primer\n", i);
    }
    //退出线程向pthread_join返回结构体指针p
    pthread_exit(p);
}


int main () {
    int err;
    pthread_t tid[THRNUM];
    struct num* p = NULL;
    //创建多个线程，每个线程判断一个数是否为素数
    for (int i = LEFT; i <= RIGHT; ++i) {
        p = malloc(sizeof(*p));
        if (p == NULL) {
            perror("malloc()");
            exit(1);
        }
        p->n = i;
        err = pthread_create(tid+(i-LEFT), NULL, thr_primer, p);
        if (err) {
            fprintf(stderr, "pthread_create error: %d", err);
            exit(1);
        }

    }
    
    void* ptr;
    //将所有线程join main线程，回收结构体指针p
    for (int i = 0; i < THRNUM; ++i) {
        //注意!!!!!!pthread_join第二个参数是二级指针
        pthread_join(tid[i], &ptr);
        free(ptr);
    }

    exit(0);
}