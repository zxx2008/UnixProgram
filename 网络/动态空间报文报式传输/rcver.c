/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-11 20:05:22
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-12 16:57:33
 * @FilePath: /网络/动态空间报文报式传输/rcver.c
 * @Description: 动态报文的接受方，接受方不知道要接受报文的具体大小，已最大值申请内存
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "proto.h"

#define IPSTRSIZE 64

int main() {
    //套接字
    int sk = socket(AF_INET, SOCK_DGRAM, 0);

    if (sk < 0) {

    }

    //本机地址，对端地址
    struct sockaddr_in laddr, raddr;
    
    //msg_st结构体指针
    struct msg_st* rbuf;
    //msg_st指针申请最大内存
    rbuf = (struct msg_st*)malloc(sizeof(struct msg_st) + NAMEMAX - 1); //-1为NAME[1]所占的大小
    //bind，判断错误
    laddr.sin_port = htons(atoi(RCVPORT));
    laddr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &(laddr.sin_addr.s_addr));
    if (bind(sk, (void*)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        close(sk);
        exit(1);
    }
    //循环接收报文，判断错误
    socklen_t raddr_len = sizeof(raddr);
    char ipstr[IPSTRSIZE];
    while (1) {
        if (recvfrom(sk, rbuf, sizeof(*rbuf), 0, (void*)&raddr, &raddr_len) < 0) {
            perror("recvfrom()");
            close(sk);
            exit(1);
        }
        inet_ntop(AF_INET, &raddr.sin_addr.s_addr, ipstr, IPSTRSIZE);
        printf("--MESSAGE FROM %s : %d--\n", ipstr, ntohs(raddr.sin_port));
        printf("Name = %s\n", rbuf->name);
        printf("Math = %d\n", ntohl(rbuf->math));
        printf("Chinese = %d\n", ntohl(rbuf->chinese));
    }
    

    //关闭套接字
    close(sk);
    exit(0);
}