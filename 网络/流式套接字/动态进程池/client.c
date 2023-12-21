/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-20 21:46:25
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-20 21:46:21
 * @FilePath: /静态进程池/client.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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
        fprintf(stderr, "fscanf() failed\n");
        exit(1);
    }
    else {
        printf("stamp = %lld\n", stamp);
    }
    fclose(fp);
    close(sd);
    exit(0);
    
}
