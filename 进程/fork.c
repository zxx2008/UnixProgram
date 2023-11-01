//fork函数多进程实现找出30000000~30000200的所有质数
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LEFT 30000000
#define RIGHT 30000200

int main() {
    //mark为标记为，1为合数，0为质数
    int i, j, mark;
    pid_t pid;
    for (i = LEFT; i < RIGHT; ++i) {
        //先设置mark为1，后面如果判断mark为合数将mark赋值为0
        //mark = 1;
        pid = fork();
        if (pid < 0) {
            perror("fork()");
            exit(1);
        }
        if (pid == 0) {//子进程
            mark = 1;
            for (j = 2; j < i/2; ++j) {
                if (i % j == 0) {
                    mark = 0;
                    break;
                }
            }
            if (mark == 1) {
                printf("%d is a primer.\n", i);
            }
            exit(0); //子进程退出
        }
        
    }
    exit(0);
} 

