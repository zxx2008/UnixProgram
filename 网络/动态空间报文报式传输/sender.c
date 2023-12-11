/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-11 19:55:50
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-11 21:40:12
 * @FilePath: /网络/动态空间报文报式传输/sender.c
 * @Description: 发送方发送的结构体大小和内容是动态的，根据用户的命令行参数来确定
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>

#include "proto.h"

int mai(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: ./sender <ip> <name>\n");
        exit(1);
    }
    //套接字
    int sk = socket(AF_INET, SOCK_DGRAM, 0);
    if (sk < 0) {
        perror("socket()");
        exit(1);
    }
    //msg_st结构体指针
    struct msg_st* sbuf;
    //申请动态内存
    size_t size = sizeof(struct msg_st) + sizeof(argv[2]) - 1;
    sbuf = (struct msg_st *)malloc(size);
    //msg_st结构体赋值
    sbuf->chinese = rand() % 100;
    sbuf->math = rand() % 100;
    strcpy(sbuf->name, argv[2]);
    //对端地址的配置s
    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, argv[1], raddr.sin_addr.s_addr);
    //sendto
    if (sendto(sk, &sbuf, sizeof(sbuf), 0, &raddr, sizeof(raddr)) < 0) {
        perror("sendto()");
        close(sk);
        exit(1);
    }
    puts("OK!");

    //关闭套接字
    close(sk);
    exit(0);
}