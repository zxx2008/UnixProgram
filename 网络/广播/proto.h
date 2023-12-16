/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-13 16:03:10
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-13 16:05:33
 * @FilePath: /网络/广播/proto.h
 * @Description: 广播udp传输，proto.h没有变化
 */

#ifndef _PROTO_H_
#define _PROTO_H_

#include <stdint.h>

#define NAMESIZE 13
#define RCVPORT "1989"

//传输的结构体
struct msg_st {
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed)); //告诉编译器，不要对齐

#endif