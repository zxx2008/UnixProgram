/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-20 20:24:58
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-20 20:24:54
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

#include "proto.h"

#define IPSTRSIZE 40
#define BUFSIZE 1024

static void server_job(int sd) {
    char buf[BUFSIZE];
    //将格式化数据写入buf中
    int len = sprintf(buf, FMT_STAMP, (long long)time(NULL));
    if (send(sd, buf, len, 0) < 0) {
        perror("send()");
        exit(1);
    }
}

int main() {
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

    //绑定地址
    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);
    if (bind(sd, (struct sockaddr*)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        close(sd);
        exit(1);
    }

    //监听连接
    if (listen(sd, 200) < 0) { //第二个参数表示建立全连接个数的最大值
        perror("listen()");
        close(sd);
        exit(1);
    }

    //接受连接
    struct sockaddr_in raddr;
    size_t raddr_len = sizeof(raddr);
    char ipstr[IPSTRSIZE];
    int newsd;
    pid_t pid;
    while(1) {
        newsd = accept(sd, (struct sockaddr*)&raddr, (socklen_t *)&raddr_len);
        if (newsd < 0) {
            perror("accept()");
            close(sd);
            exit(1);
        }
        //创建子进程
        pid = fork();
        //判断fork错误
        if(pid < 0) {
            perror("fork()");
            close(sd);
            close(newsd);
            exit(1);
        }
        //子进程
        if (pid == 0) {
            //关闭不需要的套接字
            close(sd);
            inet_ntop(AF_INET, &raddr.sin_addr.s_addr, ipstr, IPSTRSIZE);
            printf("Client: %s : %d\n", ipstr, ntohs(raddr.sin_port));
            server_job(newsd);
            close(newsd);
            //子进程结束
            exit(0);
        }
        
        close(newsd);
    }

    close(sd);
    exit(0);

}