/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-08 20:19:31
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-08 20:22:03
 * @FilePath: /网络/proto.h
 * @Description: 实现一个报式网络传输，proto.h报文格式定义
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