//使用宏来获取文件的类型权限
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//获取文件类型
static int ftype(const char* pathname) {
    struct stat buf;    //用于接收文件status
    if (stat(pathname, &buf) < 0) {
        perror("stat()");
    }
    
    if (S_ISREG(buf.st_mode))
        return '-';
    else if (S_ISDIR(buf.st_mode))
        return 'd';
    else if (S_ISSOCK(buf.st_mode))
        return 's';
    else
        return '?';
}

//获取文件所有者的权限
static int fper(const char* pathname) {

    struct stat buf;
    if (stat(pathname, &buf) < 0) {
        perror("stat()");
    }

    return buf.st_mode & S_IRWXU;
}

int main(int argc, char **argv) {
    
    if (argc < 2) {
        fprintf(stderr, "Usage: ./ftype <file>\n");
        exit(1);
    }

    printf("File type: %c\n", ftype(argv[1]));
    printf("Permission of owner: %o\n", fper(argv[1]));
    exit(0);
}