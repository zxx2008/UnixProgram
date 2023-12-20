/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-20 20:24:58
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-20 21:50:13
 * @FilePath: /并发版/server.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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

#include "proto.h"

#define IPSTRSIZE 40
#define BUFSIZE 1024
#define PROCNUM 4

static void server_job(int sd) {
    char buf[BUFSIZE];
    //将格式化数据写入buf中
    int len = sprintf(buf, FMT_STAMP, (long long)time(NULL));
    if (send(sd, buf, len, 0) < 0) {
        perror("send()");
        exit(1);
    }
}

static void server_loop(int sd) {
    //接受连接，accept是线程安全的不需要互斥量
    int new_sd;
    struct sockaddr_in raddr;
    size_t raddr_len = sizeof(raddr);
    char ipstr[IPSTRSIZE];
    while (1) {
        new_sd = accept(sd, (void *)&raddr, (void *)&raddr_len);
        if (new_sd < 0) {
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            }
            perror("accept()");
            exit(1);
        }
        inet_ntop(AF_INET, &raddr.sin_addr.s_addr, ipstr, IPSTRSIZE);
        printf("[%d]Client: %s : %d\n", getpid(), ipstr, ntohs(raddr.sin_port));
        //处理任务
        server_job(new_sd);
        close(new_sd);
    }

    close(sd);
    
}

int main() {
    
    //创建套接字
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }
    
    //设置端口复用
    int val = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        perror("setsockopt()");
        close(sd);
        exit(1);
    }

    //套接字绑定地址
    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);
    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        close(sd);
        exit(1);
    }

    //监听
    if (listen(sd, 100) < 0) {
        perror("listen()");
        close(sd);
        exit(1);
    }

    //创建若干个子进程作为进程池
    pid_t pid;
    for (int i = 0; i < PROCNUM; ++i) {
        
        pid = fork();

        if (pid < 0) {
            perror("fork()");
            close(sd);
            exit(1);
        }

        if (pid == 0) {
            server_loop(sd);
            exit(0);
        }
    }

    for (int i = 0; i < PROCNUM; ++i) {
        wait(NULL);
    }

    close(sd);
    exit(0);


}






