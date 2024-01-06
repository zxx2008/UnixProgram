/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-25 21:52:15
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-01-06 16:57:39
 * @FilePath: /src/include/proto.h
 * @Description: 定义传输协议
 */
#ifndef _PROTO_H_
#define _PROTO_H_

#include "site_type.h"

#define DEFAULT_MGROUP  "224.2.2.2" //多播组地址
#define DEFAULT_RCVPORT "1989"  //服务端端口号

#define CHNNR 100 //频道数量的最大值
#define LISTCHNID 0 //0号频道作为发送节目单
#define MINCHNID  1 //最小的频道号
#define MAXCHNID  (MINCHNID + CHNNR - 1)

#define MSG_CHANNEL_MAX (65536 - 20 - 8)    //msg_channel_st结构体的大小
#define MAX_DATA    (MSG_CHANNEL_MAX - sizeof(chnid_t)) //msg_channel_st结构体中data的大小上限
#define MSG_LIST_MAX (65536 - 20 - 8)   //msg_listentry_st结构体的最大值
#define MAX_ENTRY    (MSG_LIST_MAX - sizeof(chnid_t))   //msg_listentry_st结构体description的最大值


/**
 * @description: 频道数据包结构体
 */
struct  msg_channel_st
{
    chnid_t chnid;  //MINCHNID与MAXCHNID之间
    uint8_t data[1];
}__attribute__((packed));

/**
 * @description: 1  music: ...
 *               2  sport: ...
 */

struct msg_listentry_st
{
    chnid_t chnid;  //频道号
    uint16_t len;   //结构体长度
    uint8_t desc[1];    //频道描述
}__attribute__((packed));


struct msg_list_st {
    chnid_t chnid;      //LISTCHNID频道号
    struct msg_listentry_st entry[1];    //频道号及其描述
}__attribute__((packed));

#endif