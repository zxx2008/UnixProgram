/*使用匿名管道进行进程间通信,父进程写，子进程读*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1024

int main () {
    //定义长度为2的数组表示写端和读端的文件描述符，定义buf
    int pd[2];
    char buf[BUFSIZE];
    pid_t pid;

    //初始化匿名管道，判断出错
    if (pipe(pd) < 0) {
        perror("pipe()");
        exit(1);
    }

    
    //fork()，判断出错
    if ((pid = fork()) < 0) {
        perror("fork()");
        close(pd[0]);
        close(pd[1]);
    }

    //pid == 0表示子进程，关闭写端
    if(pid == 0) {
        close(pd[1]);
        //读
        int len = read(pd[0],buf, BUFSIZE);
        //标准输出
        puts(buf);
            //关闭读端，管道是fork之前创建的，父子进程里都有一份，所以退出之前要确保管道两端都关闭
        close(pd[0]);
        exit(0);
    }
    
    else {

        //pid > 0 表示父进程，关闭读端
        close(pd[0]);
        //写
        int len = write(pd[1], "Hello", 6);

        //关闭写端
        close(pd[1]);

        //回收子进程
        wait(NULL);

        exit(0);
    }
    exit(0);
}
