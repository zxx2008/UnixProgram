#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "mytbf.h"

#define CPS 10  //流速
#define BURST 100   //最大令牌数
#define SIZE 1024 //每次读取数量

int main(int argc, char** argv) {
    
    if (argc < 2) {
        fprintf(stderr, "Usage: ./a.out <filename>\n");
        exit(1);
    }

    mytbf_t* obj = mytbf_init(CPS, BURST);
    if (obj == NULL) {
        fprintf(stderr, "tbf init error\n");
        exit(1);
    }

    //打开文件
    int sd;
    int dsd = 0;    //标准输出
    while ((sd = open(argv[1], O_RDONLY)) < 0) {
        if (errno != EINTR) {
            perror("open()");
            exit(1);
        }
    }

    char buf[SIZE];
    int num; //消耗的令牌数
    int len, ret, pos;
    while(1) {
        num = mytbf_fetch_token(obj, SIZE);
        if (num < 0) {
            fprintf(stderr, "%s\n", strerror(-num));
            exit(1);
        }

        //read
        while((len = read(sd, buf, num)) < 0) {
            if (errno != EINTR) {
                perror("read()");
                close(sd);
                exit(1);
            }
        }
        
        if (len == 0) {
            break;
        }

        //未用完的令牌返还
        if (len < num) {
            mytbf_return_token(obj, num - len);
        }

        //write
        pos = 0;
        while (len > 0) {
            while((ret = write(dsd, buf + pos, len)) < 0) {
                if (errno != EINTR) {
                    perror("write()");
                    close(sd);
                    exit(1);
                }
            }
            len -= ret;
            pos += ret;
        }       
    }

    close(sd);
    exit(0);
}