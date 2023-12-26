/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-26 17:22:44
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-26 17:46:53
 * @FilePath: /src/client/client.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _CLIENT_H_
#define _CLIENT_H_

#define DEFAULT_PLAYERCMD "/usr/bin/mpg123 > /dev/null"

/**
 * @description: 客户端配置信息结构体
 */
struct client_conf_st {
    char* rcvport;  //端口
    char* mgroup;   //多播组地址
    char* player_cmd;   //播放器命令
};

extern struct client_conf_st client_conf;

#endif