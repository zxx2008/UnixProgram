/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-11 19:10:21
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-11 20:36:18
 * @FilePath: /网络/动态空间报文报式传输/proto.h
 * @Description: 报文具有动态空间大小的报式传输
 */

#ifndef _PROTO_H_
#define _PROTO_H_

#include <stdint.h>

#define RCVPORT "1989"
#define NAMEMAX (512-8-8) //512为udp包推荐的字节数，8为udp包的报头大小，8为结构体中固定长度即math和chinese的长度

//传输的变长结构体
struct msg_st {
    uint32_t math;
    uint32_t chinese;
    uint8_t name[1];   //1个空间作为占位符
}__attribute__((packed));

#endif
