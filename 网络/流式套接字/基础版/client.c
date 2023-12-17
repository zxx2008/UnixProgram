/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-16 16:02:03
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-17 20:13:38
 * @FilePath: /网络/流式套接字/基础版/basic.c
 * @Description: 流式套接字基础版
 * 
 * Client端（主动端）
 * 1. 获取SOCKET
 * 2. 给SOCKET取得地址（可省）
 * 3. 发送连接
 * 4. 收/发消息
 * 5. 关闭
 * 
 * Server端
 * 1. 获取SOCKET
 * 2. 给SOCKET取得地址
 * 3. 将SOCKET置为监听模式
 * 4. 接受连接
 * 5. 收/发消息
 * 6. 关闭
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "proto.h"

int main(int argc, char** argv) {
    
    if (argc < 2) {
        fprintf(stderr, "Usage: ./client <ip>\n");
        exit(1);
    }
    //套接字
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }
    //对端地址配置
    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET, argv[1], &raddr.sin_addr.s_addr);

    //connect
    if (connect(sd, (void *)&raddr, sizeof(raddr)) < 0) {
        perror("connect()");
        close(sd);
        exit(1);
    }

    //系统io转标准io
    FILE* fp;
    fp = fdopen(sd, "r+");
    if (fp == NULL) {
        perror("fdopen()");
        close(sd);
        exit(1);
    }
    //从FILE流中获得时间戳
    long long stamp;
    if (fscanf(fp, FMT_STAMP, &stamp) < 0) {
        perror("fscanf()");
        exit(1);
    }
    else {
        printf("stamp = %lld\n", stamp);
    }
    fclose(fp);
    close(sd);
    exit(0);
    
}
