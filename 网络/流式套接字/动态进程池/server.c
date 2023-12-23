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


#define IPSTRSIZE 40
#define BUFSIZE 1024
#define MINSPARESERVER 5  //最小可空闲进程
#define MAXSPARESERVER 10   //最大可空闲进程
#define MAXCLIENT 20    //进程资源总量
#define SIG_NOTIFY SIGUSR2  //自定义的信号
#define LINEBUFFSIZE 1024

static struct server_st * serverpool;   //server_st数组的首地址
static int idle_count = 0, busy_count = 0;  //计数器空闲进程数量，非空闲进程数量
static int sd; //监听套接字


//进程状态枚举
enum {
    STATE_IDLE = 0,
    STATE_BUSY
};

//包含进程信息的结构体
struct server_st {
    pid_t pid;  //进程pid
    int state;  //进程状态
};

static int scan_pool(void);

//信号SIG_NOTIFY处理状态
static void usr2_handler(int s) {
    scan_pool();
    return;
}

//子进程任务
static void server_job(int pos) {
    //对端地址结构体
    struct sockaddr_in raddr;
    socklen_t raddr_len = sizeof(raddr);
    char ipstr[IPSTRSIZE];
    time_t stamp;
    //父进程pid
    pid_t ppid = getppid();
    //建立连接后用于通信的套接字
    int client_sd;
    //消息字符串
    char lineBuff[LINEBUFFSIZE];
    int len;

    //循环等待连接，状态改变需要向主进程发送信号SIG_NOTIFY，通知主进程扫描目前进程
    //池中的状态
    while (1) {
        serverpool[pos].state = STATE_IDLE;
        //状态改变向主进程发信号
        kill(ppid, SIG_NOTIFY);
        client_sd = accept(sd, (void *)&raddr, &raddr_len);
        if (client_sd < 0) {
            perror("accept()");
            munmap(serverpool, sizeof(struct server_st) * MAXCLIENT);
            close(sd);
            exit(1);
        }

        //完成连接，设置状态为忙碌态
        serverpool[pos].state = STATE_BUSY;
        //向主进程发信号
        kill(ppid, SIG_NOTIFY);
        inet_ntop(AF_INET, &raddr.sin_addr.s_addr, ipstr, IPSTRSIZE);
        stamp = time(NULL);
        printf("[%d]Client: %s : %d", getpid(), ipstr, ntohs(raddr.sin_port));
        len = snprintf(lineBuff, LINEBUFFSIZE, FMT_STAMP, (long long)stamp);
        send(client_sd, lineBuff, len, 0);
        sleep(5);
        close(client_sd);
    }
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
    for (slot = 0; slot < MAXCLIENT; ++slot) {
        if (serverpool[slot].pid == -1) {
            break;
        }
    }
    // 新增进程状态设置为空闲
    serverpool[slot].state = STATE_IDLE;
    //创建子进程，子进程负责干活，父进程设置子进程的pid
    pid = fork();
    if (pid < 0) {
        perror("fork()");
        close(sd);
        munmap(serverpool, sizeof(struct server_st) * MAXCLIENT);
        exit(1);
    }
    if (pid == 0) {
        //子进程干活
        sleep(2);
        server_job(slot);
        exit(0);
    }
    else {
        serverpool[slot].pid = pid;
        idle_count++;
    }
    return 0;
}

//删除一个子进程
static int del_1_server(void) {
    if (idle_count == 0) {
        return -1;
    }
    //找到一个空闲进程删除
    for (int i = 0; i < MAXCLIENT; ++i) {
        if (serverpool[i].pid != -1 && serverpool[i].state == STATE_IDLE) {
            kill(serverpool[i].pid, SIGTERM);
            serverpool[i].pid = -1;
            idle_count--;
            break;
        }
    }

    return 0;
}

//扫描进程池统计idle_count和busy_count
static int scan_pool(void) {
    int busy = 0;
    int idle = 0;
    for (int i = 0; i < MAXCLIENT; ++i) {
        if (serverpool[i].pid == -1) {
            continue;
        }
        if (kill(serverpool[i].pid, 0) == -1) {
            serverpool[i].pid = -1;
            continue;
        }
        if (serverpool[i].state == STATE_IDLE) {
            idle++;
        }
        else if (serverpool[i].state == STATE_BUSY) {
            busy++;
        }
        else {
            fprintf(stderr, "Unknown state.\n");
            abort();
        }
    }
    idle_count = idle;
    busy_count = busy;
    printf("idle_count = %d\n", idle_count);
    printf("busy_count = %d\n", busy_count);

    return 0;
}


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

    int val = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        perror("setsockopt()");
        close(sd);
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
    for (int i = 0; i < MINSPARESERVER; ++i) {
        add_1_server();
    }

    scan_pool();
    
    while (1) {
        // 信号驱动：当子进程的状态发生变化时，向父进程发送信号，父进程相应作出一些动作
        // 解除对SIG_NOTIFY的屏蔽，并且阻塞在这里，直到子进程调用kill，发送给主进程一个信号来打断此阻塞，使得主进程能够向下执行
        sigsuspend(&oset);

        //扫描进程池：多退少补
        if (idle_count > MAXSPARESERVER) {
            for (int i = 0; i < (idle_count - MAXSPARESERVER); ++i) {
                del_1_server();
            }
        }
        else if (idle_count < MINSPARESERVER) {
            for (int i = 0; i < (MINSPARESERVER - idle_count); ++i) {
                add_1_server();
            }
        }

        //输出进程池的状态
        for (int i = 0; i < MAXCLIENT; ++i) {
            //未分配进程
            if (serverpool[i].pid == -1) {
                putchar('x');
            }
            //空闲态进程
            else if(serverpool[i].state == STATE_IDLE) {
                putchar('.');
            }
            //忙碌态进程
            else {
                putchar('$');
            }
        }
        putchar('\n');

    }

    sigprocmask(SIG_SETMASK, &oset, NULL);
    close(sd);
    munmap(serverpool, sizeof(struct server_st) * MAXCLIENT);
    exit(0);

}






