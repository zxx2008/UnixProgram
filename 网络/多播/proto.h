/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-13 20:13:04
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-13 20:16:25
 * @FilePath: /网络/多播/proto.h
 * @Description: udp多播组传输，proto.h约定多播地址
 */

#ifndef _PROTO_H_
#define _PROTO_H_

#include <stdint.h>

#define NAMESIZE 13
#define RCVPORT "1989"
#define MTGOURP "224.2.2.2" //约定的多播组ip

//传输的结构
struct msg_st {
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed)); //告诉编译器，不要对齐

#endif