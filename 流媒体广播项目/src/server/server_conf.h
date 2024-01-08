/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-28 13:28:21
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-01-07 21:29:51
 * @FilePath: /src/server/server_conf.h
 * @Description: 服务器端配置信息
 */
#ifndef _SERVER_CONF_H_
#define _SERVER_CONF_H_

#define DEFAULT_MEDIADIR "/var/media" //默认媒体库
#define DEFAULT_IF "eth0" //默认网卡

enum {
    RUN_DAEMON = 1,
    RUN_FOREGROUND
};

struct server_conf_st {
    char* rcvport; //端口
    char* mgroup; //多播组地址
    char* media_dir; //指定媒体库
    char* runmode; //前台运行或后台运行
    char* ifname; //指定网卡
};

extern struct server_conf_st server_conf;
extern int server_sd;
extern struct sockaddr_in sndaddr; 

#endif