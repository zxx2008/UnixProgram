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

#define IPSTRSIZE 64

int main() {
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
    inet_pton()
    //循环接受

    //关闭套接字
}