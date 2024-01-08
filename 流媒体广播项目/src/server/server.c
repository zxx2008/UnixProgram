/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-28 12:54:10
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-01-07 22:06:44
 * @FilePath: /src/server/server.c
 * @Description: 服务器端main文件
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "server_conf.h"
#include "../include/proto.h"
#include "medialib.h"
#include "thr_list.h"


/*
* -M 指定多播组
* -P 指定接受端口
* -F 前台运行
* -D 指定媒体库的位置
* -I 指定网络设备
* -H 显示帮助
*/

// 服务端配置信息
int server_sd;  //服务端套接字

struct sockaddr_in sndaddr; //对端地址

struct server_conf_st server_conf = {.rcvport = DEFAULT_RCVPORT, \
.mgroup = DEFAULT_MGROUP, \
.media_dir = DEFAULT_MEDIADIR, \
.runmode = RUN_DAEMON, \
.ifname = DEFAULT_IF};

// 打印帮助
static void printfHelp(void) {
    printf("-M 指定多播组\n-P 指定接受端口\n-F 指定前台运行\n-D 指定媒体库的位置\n-I 指定网络设备\n-H 显示帮助\n");
}

// 守护进程结束
static void daemon_exit(int s) {
    ;
}

// 守护进程, 脱离终端
static int daemonize(void) {
    pid_t pid;
    pid = fork(); 
    if(pid < 0) {
        // perror("fork()");
        // 写系统日志，LOG_ERR 错误级别
        syslog(LOG_ERR, "fork(): %s", strerror(errno));
        return -1;
    }
    if(pid > 0)  //父进程
        exit(0);

    int fd = open("/dev/null", O_RDWR);
    if(fd < 0) {
        //perror("open()");
        syslog(LOG_WARNING, "open(): %s", strerror(errno));
        return -2;
    }
    else {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        if (fd > 2) {
            close(fd);
        }
    }
    
    setsid();   //守护进程
    chdir("/"); //改变工作目录
    umask(0);

    return 0;
}

// 初始化端口
static int socket_init(void) {
    server_sd = socket(AF_INET, SOCK_DGRM, 0);
    if(server_sd < 0) {
        syslog(LOG_ERR, "socket():%s", strerror(errno));
        exit(1);
    }
    // 设置端口属性
    struct ip_mreqn mreq;
    inet_pton(AF_INET, server_conf.mgroup, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex(server_conf.ifname);
    if(setsockopt(server_sd, IPPROTO_IP, IP_MULTICAST_IF) < 0) {
        syslog(LOG_ERR, "setsockopt(IP_MULTICAST_IF):%s", strerror(errno));
        exit(1);
    }
    // bind 可省略

    sndaddr.sin_family = AF_INET;
    sndaddr.sin_port = htons(atoi(server_conf.rcvport));
    inet_pton(AF_INET, server_conf.mgroup, sin_addr.s_addr);
    return 0;
}


int main(int argc, char** argv) {
    /*
    * 命令行分析
    */
    int c;

    // 定义信号处理函数
    struct sigaction sa;
    sa.sa_handler = daemon_exit;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaddset(&sa.sa_mask, SIG)
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);


    //系统日志
    openlog("netradio", LOG_PID | LOG_PERROR, LOG_DAEMON);

    while(1) {
        c = getopt(argc, argv, "M:P:FD:I:H");
        if(c < 0) {
            break;
        }
        switch (c)
        {
        case 'M':
            server_conf.mgroup = optarg;
            break;
        case 'P':
            server_conf.rcvport = optarg;
            break;
        case 'F':
            server_conf.runmode = RUN_FOREGROUND;
            break;
        case 'D':
            server_conf.media_dir = optarg;
            break;
        case 'I':
            server_conf.ifname = optarg;
            break;
        case 'H':
            printfHelp();
            exit(0);
            break;
        default:
            abort();
            break;
        }
    }

    // 守护进程的实现
    
    if(server_conf.runmode == RUN_DAEMON) {
        if(daemonize() < 0) {
            perror("daemonize()");
            exit(1);
        }
    }
    else if (server_conf.runmode == RUN_FOREGROUND) {

    }
    else {
        //fprintf(stderr, "EINVAL\n");
        syslog(LOG_ERR, "EINVAL server_conf.runmode.");
        exit(1);
    }

    // socket初始hua
    socket_init();

    // 获取频道信息
    struct mlib_listentry_st *list;
    int list_size;
    int err;
    err = mlib_getchnlist(&list, &list_size);

    // 创建节目单频道线程
    thr_list_create(list, list_size);

    // 创建其他频道线程
    int i;
    for(i = 0; i < list_size; ++i) {
        thr_channel_create(list + i);
        /*if error*/
    }

    syslog(LOG_DEBUG, "%d channel threads created", i);
    //closelog();
}