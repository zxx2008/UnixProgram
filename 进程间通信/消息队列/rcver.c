/*使用消息队列，接收端*/
#include "proto.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main() {
    //消息队列key
    key_t key = ftok(KEYPATH, KEYPROJ);
    if (key < 0) {
        perror("ftok()");
        exit(0);
    }
    //接收端先启动，由接收端创建消息队列
    int msgQueneId = msgget(key, IPC_CREAT|0600);
    if (msgQueneId < 0) {
        perror("msgget()");
        exit(0);
    }

    struct msg_st msgbuf;
    //不停的接受消息
    while(1) {
        if (msgrcv(msgQueneId, &msgbuf, sizeof(msgbuf)- sizeof(long), 0, 0) < 0) {
            if (errno == EINTR) {
                continue;
            }
            else {
                perror("msgrcv()");
                exit(0);
            }
        }
        //输出
        if (msgbuf.mtype == MSGTYPE) {
            printf("Name = %s\n", msgbuf.name);
            printf("Math = %d\n", msgbuf.math);
            printf("Chinese = %d\n", msgbuf.chinese);
        }
    }

    //销毁消息队列

    msgctl(msgQueneId, IPC_RMID, NULL);

    exit(0);
}