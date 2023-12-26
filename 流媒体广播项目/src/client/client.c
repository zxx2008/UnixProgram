/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-25 22:30:49
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-26 17:45:36
 * @FilePath: /src/client/client.c
 * @Description: 客户端，主进程使用socket接受数据，交给子进程进行解码播放，父子进程通信使用管道进行
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <proto.h>
#include "client.h"

/*
* -M --mgroup 指定多播组
* -P --port 指定接收器端口
* -p --player 指定播放器
* -H --help   显示帮助
*/

//默认客户端设置信息
struct client_conf_st client_conf = {\
 .rcvport = DEFAULT_RCVPORT, \
 .mgroup = DEFAULT_MGROUP, \
 .player_cmd = DEFAULT_PLAYERCMD \
};

static void printHelp(void) {
    printf("-P --port    指定接受端口\n-M --mgroup    指定多播组地址\n-p --player    指定播放器命令行\n-H --help    显示帮助");
}

int main(int argc, char** argv) {

    //分析命令行参数
    int index = 0;
    struct option argarr[] = {{"port", 1, NULL, 'P'}, \
    {"mgroup", 1, NULL, 'M'}, \
    {"player", 1, NULL, 'p'}, {"help", 0, NULL, 'H'}, \
    {NULL, 0, NULL, 0}};
    //man getopt_long 获得详细信息

    int c;
    while (1) {
        c = getopt_long(argc, argv, "P:M:p:H", argarr, &index);
        if (c < 0) {
            break;
        }
        switch (c)
        {
            case 'P':   //PORT
                client_conf.rcvport = optarg;
                break;
            case 'M':   //mgroup
                client_conf.mgroup = optarg;
                break;
            case 'p':   //player
                client_conf.player_cmd = optarg;
                break;
            case 'H':   //help
                //printf("");
                printHelp();
                exit(0);
                break;
            default:
                printf("不合法的参数\n");
                abort();
                break;
        }
    }
    
    
    //socket
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    //设置客户端加入多播组 (IP_DROP_MEMBERSHIP 离开多播组)
    struct ip_mreqn mreq;
    if(inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr) < 0) {
        perror("inet_pton()");
        close(sd);
        exit(1);
    }

    if(inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address) < 0) {
        perror("inet_pton()");
        close(sd);
        exit(1);
    }
    mreq.imr_ifindex = if_nametoindex("eth0");
    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt()");
        close(sd);
        exit(1);
    }

    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(client_conf.rcvport));
    inet_pton(AF_INET, "0.0.0.0", laddr.sin_addr.s_addr);
    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        close(sd);
        exit(1);
    }
}