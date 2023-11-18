/*使用漏斗完成每秒读十个字符的cat命名*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define CPS 10
#define BUFSIZE CPS

//定义一个易变的全局变量
static volatile int loop = 0;

//信号处理函数
static void alrm_handler(int s) {
    alarm(1);
    loop = 1;
}

int main(int argc, char** argv) {
    //定义两个文件描述符
    int sfd, dfd = 1;
    //定义缓冲区
    char buf[BUFSIZE];

    int len, pos, ret;

    //判断主函数参数个数
    if (argc < 2) {
        fprintf(stderr, "Usage ./slowcat <file name>\n");
        exit(1);
    }

    //定义alarm信号处理
    signal(SIGALRM, alrm_handler);
    //alarm信号，1秒后发出
    alarm(1);

    //打开文件argv[1]
    do {
        if ((sfd = open(argv[1], O_RDONLY)) < 0) {
            if (errno != EINTR) {
                perror("open()");
                exit(1);
            }
        }
    }while(sfd < 0);

    //循环读取文件中的类容
    while(1) {
        //判断全局变量loop
        while(!loop) {
            //防止cpu空转，pause()将当前进程中断知道收到alarm信号
            pause();
        }
        //将loop置为0
        loop = 0;

        //读取到buf中
        while ((len = read(sfd, buf, BUFSIZE)) < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("read()");
            break;
        }

        if (len == 0)
            break;

        pos = 0;

        //向标准输出写        
        while (len > 0) {
            if ((ret = write(dfd, buf + pos, len)) < 0) {
                if (errno == EINTR) {
                    continue;
                }
                perror("write()");
                close(sfd);
                exit(1);
            }
            
            pos += ret;
            len -= ret;
        }
    }

    close(sfd);
    exit(0);
}