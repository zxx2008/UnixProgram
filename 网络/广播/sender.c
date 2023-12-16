/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-13 16:02:32
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-13 16:22:13
 * @FilePath: /网络/广播/sender.c
 * @Description: 广播模式发送方，需要设置socket状态,并且对端ip为255.255.255.255 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "proto.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        perror("Usage: ./sender <message>");
        exit(0);
    }
    //创建套接字
    int sk = socket(AF_INET, SOCK_DGRAM, 0);
    if (sk < 0) {
        perror("socket()");
        exit(1);
    }

    //设置广播模式
    int val = 1;
    if(setsockopt(sk, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val)) < 0) {
        perror("setsockopt()");
        close(sk);
        exit(1);
    }

    //发送的结构体
    struct msg_st wbuf;
    strcpy(wbuf.name, argv[1]);
    wbuf.math = htonl(rand() % 100);
    wbuf.chinese = htonl(rand() % 100);
    //bind 主动端可省略绑定端口的步骤
    //对端地址的配置
    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, "255.255.255.255", (void*)&raddr.sin_addr.s_addr);

    //发送数据
    if ((sendto(sk, (void*)&wbuf, sizeof(wbuf), 0, (struct sockaddr*)&raddr, sizeof(raddr))) < 0) {
        perror("sendto()");
        close(sk);
        exit(1);
    }

    puts("ok");

    //关闭套接字
    close(sk);
    exit(0);
}