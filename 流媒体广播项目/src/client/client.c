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
#include <errno.h>

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

static ssize_t writen(int fd, const char* buf, size_t len) {
    int pos = 0;
    int ret;
    while(len > 0) {
        ret = write(fd, buf + pos, len);
        if (ret < 0) {
            if(errno == EINTR) continue;
            perror("write()");
            return -1;
        }

        len -= ret;
        pos += ret;
    }
    return pos;
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

    //设置套接字属性
    int val = 1;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val)) < 0) {
        perror("setsockopt()");
        close(sd);
        exit(1);
    }

    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(client_conf.rcvport));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);
    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        close(sd);
        exit(1);
    }

    //管道用于父子进程通信
    int pd[2];
    if (pipe(pd) < 0) {
        perror("pipe()");
        close(sd);
        exit(1);
    }

    //创建子进程，父进程网络收包，子进程调用解码器
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork()");
        close(sd);
        exit(1);
    }

    if (pid == 0) {
        // 子进程调用解码器
        close(sd);  //关闭套接字，子进程不需要
        close(pd[1]); //关闭管道写端
        dup2(pd[0], 0); //将管道输入重定向到标准输入
        if (pd[0] > 0) {
            close(pd[0]);   
        }
        execl("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL);//调用解码器
        perror("execl()");
        close(pd[1]);
        exit(1);
    }

    // 收节目单
    struct msg_list_st* msg_list;
    msg_list = malloc(MSG_LIST_MAX);
    if(msg_list == NULL) {
        perror("malloc()");
        close(sd);
        close(pd[0]);
        close(pd[1]);
        exit(1);
    }

    ssize_t len;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    while (1)
    {
        len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void *)&server_addr, &server_addr_len);
        if (len < sizeof(struct msg_list_st )) {
            fprintf(stderr, "message is too small.\n");
            continue;
        }

        if (msg_list->chnid != LISTCHNID) {
            fprintf(stderr, "chnid is not match.\n");
            continue;
        }

        break;

    }
    
    //打印节目单并选择频道
    struct msg_listentry_st *pos;
    for (pos = msg_list->entry; (char*)pos < ((char*)msg_list + len); pos = (void*)((char*)pos + ntohs(pos->len))) {
        printf("%d : %s\n", pos->chnid, pos->desc);
    }
    exit(0);

    free(msg_list);

    // 选择频道
    int chosen_id;
    int ret;
    while(1) {
        ret = scanf("%d", &chosen_id);
        if (ret != 1) {
            exit(1);
        }
    }

    // 接受频道包
    struct msg_channel_st * msg_channel;
    msg_channel = malloc(MSG_CHANNEL_MAX);
    if(msg_channel == NULL) {
        perror("malloc()");
        close(sd);
        close(pd[0]);
        close(pd[1]);
        exit(1); 
    }

    struct sockaddr_in raddr;
    socklen_t raddr_len = sizeof(raddr);
    while(1) {
        len = recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (void*)&raddr, &raddr_len);
        
        if(raddr.sin_addr.s_addr != server_addr.sin_addr.s_addr || raddr.sin_addr.s_addr != server_addr.sin_addr.s_addr) {
            fprintf(stderr, "Ignore: address is not match.\n");
            continue;
        }

        if(len < sizeof(struct msg_channel_st)) {
            fprintf(stderr, "Ignore: message too small.\n");
            continue;
        }

        if(msg_channel->chnid == chosen_id) {
            fprintf(stdout, "accepted msg: %d recieved.\n", msg_channel->chnid);
            if(writen(pd[1], msg_channel->data, (len - sizeof(chnid_t))) < 0) {
                exit(1);
            }
        }
    }

    free(msg_channel);
    close(sd);

    exit(0);
    
}