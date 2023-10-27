//使用opendir,readdir获取目录中的文件名
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#define PATH "/home/zxx"

int main(int argc, char** argv) {

    DIR* pdir = opendir(PATH);

    //打开目录失败的情况
    if (pdir == NULL) {
        perror("opendir()");
    }

    struct dirent* curpdirent;
    
    while ((curpdirent = readdir(pdir)) != NULL) {
        printf("file name: %s\n", curpdirent->d_name);
    }

    closedir(pdir);
}