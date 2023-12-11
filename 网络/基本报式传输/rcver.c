/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-08 20:32:47
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-08 21:41:27
 * @FilePath: /网络/rcver.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "proto.h"
#include <arpa/inet.h>
#include <unistd.h>

#define IPSTRSIZE 64

int main() {
    //用于保存发送方ip地址的字符串,必须是数组，之前定义为char* 报段错误
    char ipstr[IPSTRSIZE];
    //套接字，IPV4的报式tao'jie套接字，0为默认协议，即UDP
    int sk = socket(AF_INET, SOCK_DGRAM, 0);
    // 判断错误
    if (sk < 0) {
        perror("socket()");
        exit(0);
    }
    //本机地址配置
    struct sockaddr_in laddr, raddr;
    //关联地址与套接字
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);
    if (bind(sk, (struct sockaddr*)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        close(sk);
        exit(1);
    }
    //接受的消息结构体对象
    struct msg_st rbuf;
    //循环接受
    socklen_t raddr_len = sizeof(raddr);
    while(1) {
        if ((recvfrom(sk, &rbuf, sizeof(rbuf), 0, (struct sockaddr*)&raddr, &raddr_len)) < 0) {
            perror("recvfrom()");
            close(sk);
            exit(1);
        }

        //ip地址整型转字符串
        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
        printf("---MESSAGE FROM %s %d---\n", ipstr, ntohs(raddr.sin_port));
        printf("Name = %s\n", rbuf.name);
        printf("Math = %d\n", rbuf.math);
        printf("Chinese = %d\n", rbuf.chinese);
    }
    //关闭套接字
    close(sk);
    exit(0);
}