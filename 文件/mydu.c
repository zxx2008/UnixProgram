//模仿linux du命令写mydu
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>
#include <string.h>
#include <stdint.h>

#define PATHSIZE 1024

//判断路径是否为".",".."这样的路径
static int path_no_loop(const char* path) {
    char* pos = strrchr(path, '/');
    if (pos == NULL) {
        exit(1);
    }
    if (strcmp(pos+1, ".") == 0 || strcmp(pos+1, "..") == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

//mydu的实现函数
static int64_t mydu(const char* path) {

    //定义struct stat 变量用于接收文件状态变量
    static struct stat statbuf;

    static char nextpath[PATHSIZE];

    int64_t sum;

    if (lstat(path, &statbuf) < 0) {
        perror("lstat()");   //stat失败
        exit(1);
    }

    //如果不是目录则返回文件所占blocks的数量
    if (!S_ISDIR(statbuf.st_mode)) {
        return statbuf.st_blocks;
    }

    //添加非隐藏文件到mglob中
    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/*", PATHSIZE);

    glob_t mglob;
    if (glob(nextpath, 0, NULL, &mglob) < 0) {
        perror("glob()");
        exit(1);
    }

    //添加隐藏文件到mglob中
    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/.*", PATHSIZE);

    if (glob(nextpath, GLOB_APPEND, NULL, &mglob) < 0) {
        perror("glob()");
        exit(1);
    }

    sum = statbuf.st_blocks;

    for (int i = 0; i < mglob.gl_pathc; ++i) {
        if (path_no_loop(mglob.gl_pathv[i])) sum += mydu(mglob.gl_pathv[i]);
    }

    //回收资源
    globfree(&mglob);

    return sum;
}

int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: ./mydu <file> or <directory>\n");
        exit(1);
    }

    printf("%ld\n", mydu(argv[1])/2);
    exit(0);
}