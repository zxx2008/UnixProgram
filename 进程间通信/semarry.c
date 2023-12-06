/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-05 22:03:50
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-06 21:29:34
 * @FilePath: /进程间通信/semarry.c
 * @Description: 使用信号量数组，创建20个进程每个进程的任务是对tmp文件的数值+1,信号量数组保证文件每次只能被一个进程访问
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>

#define PROCNUM 20 //子进程数量
#define FNAME "tmp.txt"
#define BUFSIZE 1024

// 多个函数都要使用这个信号量ID，所以定义为全局变量
static int semid;

//PV操作中的P，占用一个资源
static void P() {
    //通过sembuf结构体定义信号量数组操作操作
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;

    while(semop(semid, &op, 1) < 0) {
        if (errno != EINTR && errno != EAGAIN) {
            perror("semop()");
            exit(1);
        }
    }

}

//PV操作中的V，释放一个资源
static void V() {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;

    while(semop(semid, &op, 1) < 0) {
        if (errno != EINTR && errno != EAGAIN) {
            perror("semop()");
            exit(1);
        }
    }
}

//子进程函数
static void func_add() {
    FILE* fp;
    char buf[BUFSIZE];

    fp = fopen(FNAME, "r+");
    if (fp == NULL) {
        perror("fopen()");
        exit(1);
    }

    P();
    fgets(buf, BUFSIZE, fp);
    rewind(fp);
    fprintf(fp, "%d", atoi(buf) + 1);
    fflush(fp);
    V();

    fclose(fp);

    return;
}


int main() {
    //获得信号量id 信号量数组中只有一个信号量所以第二个参数选择1
    //具有亲缘关系的进程间通信选择IPC_PRIVATE就可以了
    int semid = semget(IPC_PRIVATE, 1, 0600);

    if (semid < 0) {
        perror("semget()");
        exit(1);
    }

    //设置信号量资源总量为1
    if (semctl(semid, 0, SETVAL, 1) < 0) {
        perror("semctl()");
        exit(1);
    }

    pid_t pid;

    //创建20个子进程
    for (int i = 0; i < 20; ++i) {
        pid = fork();
        if (pid < 0) {
            perror("fork()");
            exit(1);
            semctl(semid, 0, IPC_RMID);
        }
        //子进程
        if (pid == 0) {
            func_add();
            exit(0);
        }
    }

    //回收子进程
    for (int i = 0; i < PROCNUM; ++i) {
        wait(NULL);
    }

    exit(0);
}