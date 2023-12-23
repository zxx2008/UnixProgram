/*漏桶
需求：实现一个复制文本到标准输出的程序，要求10字符10字符的复制，且不能让CPU空转。*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define CPS 10  //每秒输出字符个数
#define BUFSIZE CPS

//判断cpu是否pause的全局变量
static int loop = 0;

//alarm信号处理函数
static void alarm_handler(int s) {
    alarm(1);
    loop = 1;
}

int main(int argc, char** argv) {
    
    if (argc < 2) {
        fprintf(stderr, "Usage: ./leaky_bucket <filename>\n");
        exit(1);
    }

    //打开文件，判断错误：真错 假错
    int sfd;
    int dfd = 1;
    do {
        if ((sfd = open(argv[1], O_RDONLY)) < 0) {
            if (errno != EINTR) {
                perror("open()");
                exit(1);
            }
        }
    } while(sfd < 0);

    //定义alarm信号行为
    signal(SIGALRM, alarm_handler);
    alarm(1);

    int len;
    int ret;
    int pos;
    char buf[BUFSIZE];
    //读写文件文件
    while (1)
    {
        while(!loop) {
            pause();
        }
        loop = 0;
        while((len = read(sfd, buf, BUFSIZE)) < 0) {
            if (errno != EINTR) {
                close(sfd);
                perror("read()");
                exit(1);
            }
        }

        if (len == 0) {
            break;
        }

        pos = 0;
        while(len > 0) {
            while ((ret = write(dfd, buf + pos, len)) < 0) 
            {
                if (errno != EINTR) {
                    perror("write()");
                    close(sfd);
                    exit(1);
                }
            }
            len -= ret;
            pos += ret;
        }
    }
    
    close(sfd);
    exit(0);
}


