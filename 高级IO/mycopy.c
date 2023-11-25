/*需求：有左右两个设备，第一个任务为读左设备，写右设备，第二个任务读右设备，写左设备。*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define BUFSIZE 1024

//状态
enum {
    STATE_R = 1,    //读
    STATEE_W,       //写
    STATE_Ex,       //异常
    STATE_T         //终止
};

//有限状态机结构体
struct fsm_st {
    int stat;   //状态
    int sfd;    //源文件
    int dfd;    //目标文件
    int len;    //读取到的数据
    int pos;    //buf的偏移量
    char buf[BUFSIZE];//缓冲区
    char* errstr;   //报错信息
};

static void fsm_driver(struct fsm_st* fsm) {
    int ret;
    //switch ()
}

//中继两个文件函数
static void relay(int fd1, int fd2) {

}

int main () {
    int fd1, fd2;   //左设备和右设备
    //阻塞模式打开左设备
    if ((fd1 = open(TTY1, O_RDWR)) < 0) {
        perror("open()");
        exit(1);
    }
    //非阻塞模式打开右设备
    if ((fd2 = open(TTY2, O_RDWR | O_NONBLOCK)) < 0) {
        perror("open()");
        exit(1);
    }

    write(fd1, "1\n", 1);
    //中继fd1与fd2
    //relay(fd1, fd2);

    close(fd1);
    close(fd2);

    exit(0);
}