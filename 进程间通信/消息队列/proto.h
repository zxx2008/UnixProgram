/*定义消息结构体*/
#ifndef _PROTO_H_
#define _PROTO_H_

#define NAMESIZE 32
#define KEYPATH "/etc/services"
#define KEYPROJ 'a'
#define MSGTYPE 10

struct msg_st {
    long mtype; //消息类型
    char name[NAMESIZE];
    //其他数据
    int math;
    int chinese;
};

#endif