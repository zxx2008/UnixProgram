/*使用消息队列，发送端*/
#include <stdio.h>
#include <stdlib.h>
#include "proto.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
//#include <time.h>
#include <string.h>
#include <errno.h>

int main() {
    //消息队列key
    key_t key = ftok(KEYPATH, KEYPROJ);
    if (key < 0) {
        perror("ftok()");
        exit(0);
    }
    //通过key取得消息队列
    int msgQueneId = msgget(key, 0);
    if (msgQueneId < 0) {
        perror("msgget()");
        exit(0);
    }
    //给要发送的结构体赋值
    struct msg_st msgbuf;
    msgbuf.mtype = MSGTYPE;
    strcpy(msgbuf.name, "XiaoMing");
    msgbuf.math = 90;
    msgbuf.chinese = 85;
    //发送结构体
    while (msgsnd(msgQueneId, &msgbuf, sizeof(msgbuf)-sizeof(long), 0) < 0) {
        if (errno == EINTR) {
            continue;
        }
        else {
            perror("msgsnd()");
            exit(0);
        }
    }
    puts("ok");
    
    exit(0);
}
