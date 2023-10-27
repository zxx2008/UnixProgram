//glob函数分析目录与读取目录内容
#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
//定义需要分析的路径，获得该路径下所有以.conf结尾的文件名
#define PATTERN "/etc/*.conf"

// If  errfunc  is  not NULL, it will be called in case of an error with the arguments epath, a pointer to the path which failed, and eerrno, the value of errno as returned from one of the calls to opendir(3), read‐dir(3), or stat(2).
int errorfunc(const char* path, int eerrno) {
    puts(path);
    fprintf(stderr, "Error: %d\n", eerrno);
    return 1;
}

int main(int argc, char **argv) {
    //用于存储目录分析结果的结构体
    glob_t mglob;

    int err = glob(PATTERN, 0, errorfunc, &mglob);
    if (err) {
        //glob返回值不为0，输出错误编号
        fprintf(stderr, "Error: %d\n", err);
        exit(1);
    }

    for (int i = 0; i < mglob.gl_pathc; ++i) {

        puts(mglob.gl_pathv[i]);

    }

    //释放资源
    globfree(&mglob);
    
    exit(0);
}   