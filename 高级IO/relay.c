/*需求：有左右两个设备，第一个任务为读左设备，写右设备，第二个任务读右设备，写左设备。使用select函数进行重构，使得在有限状态机在read阶段不会忙等*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define BUFSIZE 1024

//状态
enum {
    STATE_R = 1,    //读
    STATE_W,       //写
    STATE_AUTO,     //分隔线
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

static int max(int x, int y) {
	return x >= y ? x : y;
}

static void fsm_driver(struct fsm_st* fsm) {
    int ret;
    //switch ()
    switch (fsm->stat)
    {
    case STATE_R:
        fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
        if (fsm->len == 0) {//读完文件
            fsm->stat = STATE_T;
        }
        else if (fsm->len < 0) {
            //判断真错还是数据没有准备好
            if (errno == EAGAIN) {//数据还没准备好
                fsm->stat = STATE_R;
            }
            else {//read错误
                fsm->errstr = "read()";
                fsm->stat = STATE_Ex;
            }
        }
        else {//转换为写状态
            fsm->pos = 0;
            fsm->stat = STATE_W;
        }
        break;
    
    case STATE_W:
        //写
        ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
        //ret < 0
        if (ret < 0){
            //假错 EAGIN
            if (errno == EAGAIN) {
                fsm->stat = STATE_W;
            }
            //真错
            else {
                fsm->errstr = "write()";
                fsm->stat = STATE_W;
            }

        }

        //ret >= 0
        else {
            fsm->len -= ret;
            fsm->pos += ret;    
            //写完
            if (fsm->len == 0) {
                fsm->stat = STATE_R;
            }
            else {
                fsm->stat = STATE_W;
            }
            //未写完
        }
        break;
    case STATE_Ex:
        perror(fsm->errstr);
        fsm->stat = STATE_T;
        break;
    case STATE_T:
        break;
    
    default:
        abort();
        break;
    }
}

//中继两个文件函数
static void relay(int fd1, int fd2) {
    int fd1_save, fd2_save;
    //保存两个文件的状态选项
    fd1_save = fcntl(fd1, F_GETFL);
    fd2_save = fcntl(fd2, F_GETFL);
    //添加文件状态非阻塞模式
    fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);
    fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);
    //定义两个有限状态机分别表示从左写到右和从右写到左
    struct fsm_st fsm12, fsm21;
    fsm12.stat = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;
    
    fsm21.stat = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    fd_set reset, wset;

    while (fsm12.stat != STATE_T || fsm21.stat != STATE_T) {
        //监视任务
        FD_ZERO(&reset);
        FD_ZERO(&wset);
        if (fsm12.stat == STATE_R) {    //如果fsm12处于读状态则将sfd加入读监控集合中
            FD_SET(fsm12.sfd, &reset);
        }
        else if (fsm12.stat == STATE_W) {   //如果fsm21处于写状态则将dfd加入写监控集合中
            FD_SET(fsm12.dfd, &wset);
        }
        //对于fsm21亦然
        if (fsm21.stat == STATE_R) {
            FD_SET(fsm21.sfd, &reset);
        }
        else if (fsm21.stat == STATE_W) {
            FD_SET(fsm21.dfd, &wset);
        }

        //select监视
        if (fsm12.stat < STATE_AUTO || fsm21.stat < STATE_AUTO) {
            if (select(max(fd1, fd2) + 1, &reset, &wset, NULL, NULL) < 0) {
                if (errno == EINTR) { //假错
                    continue;
                }
                perror("select()");  //真错
                exit(1);
            }
        }
        //判断推进状态机的情况
        if (FD_ISSET(fsm12.sfd, &reset) || FD_ISSET(fsm12.dfd, &wset) || fsm12.stat > STATE_AUTO) {
            fsm_driver(&fsm12);
        }

        if (FD_ISSET(fsm21.sfd, &reset) || FD_ISSET(fsm21.dfd, &wset) || fsm21.stat > STATE_AUTO) {
            fsm_driver(&fsm21);
        }
        //fsm_driver(&fsm12);
        //fsm_driver(&fsm21);
    }

    //恢复文件原始状态
    fcntl(fd1, F_SETFL, fd1_save);
    fcntl(fd2, F_SETFL, fd2_save);

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
    relay(fd1, fd2);

    close(fd1);
    close(fd2);

    exit(0);
}
