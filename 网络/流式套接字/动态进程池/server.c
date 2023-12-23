/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-20 21:50:18
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-22 20:34:27
 * @FilePath: /动态进程池/server.c
 * @Description: 服务端使用动态进程池
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>

#include "proto.h"
#include <bits/sigaction.h>
#include <asm/signal.h>
#include <bits/mman-linux.h>

#define IPSTRSIZE 40
#define BUFSIZE 1024
#define MINSPARESERVER 5  //最小可空闲进程
#define MAXSPARESERVER 10   //最大可空闲进程
#define MAXCLIENT 20    //进程资源总量
#define SIG_NOTIFY SIGUSR2  //自定义的信号

static struct server_st * serverpool;   //server_st数组的首地址
static int idle_count = 0, busy_count = 0;  //计数器空闲进程数量，非空闲进程数量
static int sd; //监听套接字

//信号SIG_NOTIFY处理状态
static void usr2_handler(int s) {
    return;
}

//创建一个子进程
static int add_1_server(void) {
    int slot;
    pid_t pid;
    //判断当前进程数
    if (idle_count + busy_count >= MAXCLIENT) {
        //空闲和忙碌的进程数大于或等于总资源量就不增加子进程
        return -1;
    }

    // 遍历serverpool数组，查找到可用的位置创建子进程
    
    // 新增进程状态设置为空闲
    //创建子进程，子进程负责干活，父进程设置子进程的pid
}

//进程状态枚举
enum {
    STATE_IDLE = 0;
    STATE_BUSY;
};

//包含进程信息的结构体
struct server_st {
    pid_t pid;  //进程pid
    int state;  //进程状态
};


//主进程
int main() {

    // 设置SIGCHLD行为避免子进程成为僵尸进程
    struct sigaction sa, osa;
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, &osa);
    
    // 定义自定义信号的行为
    sa.sa_handler = usr2_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY, &sa, &osa);

    // 先对SIG_NOTIFY信号进行屏蔽
    sigset_t set, oset;
    sigemptyset(&set);
    sigaddset(&set, SIG_NOTIFY);
    sigprocmask(SIG_BLOCK, &set, &oset);

    //为serverpool申请20个空间， 判断是否成功
    //struct server_st * serverpool;
    serverpool = mmap(NULL, sizeof(struct server_st) * MAXCLIENT, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (serverpool == MAP_FAILED) {
        perror("mmap()");
        exit(1);
    }

    //为serverpool赋初始值
    for (int i = 0; i < MAXCLIENT; ++i) {
        serverpool[i].pid = -1;
    }

    //创建套接字， 设置端口复用
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    //本机地址配置，绑定地址
    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);
    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        close(sd);
        exit(1);
    }

    //监听连接
    if (listen(sd, 100) < 0) {
        perror("listen()");
        close(sd);
        exit(1);
    }

    //添加空闲进程，使得空闲进程数等于MINSPARESERVER
    
    

}






