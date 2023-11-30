/*./mmap <filename>获得文件中a的个数*/
/*mmap把内存中的内容或者某一个文件的内容映射到当前进程空间中*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

static int fd = -1;

static void func1() {
    if (fd > 2) {
        close(fd);
    }
}

int main(int argc, char** argv) {
    //判断参数个数
    if (argc < 2) {
        fprintf(stderr, "Usage: ./mmap <filename>");
    }
    //只读方式打开文件
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open()");
        exit(1);
    }

    //将释放文件函数挂到钩子函数上
    atexit(func1);

    //fstat获得文件状态
    struct stat statres;
    if (fstat(fd, &statres)) {
        perror("fstat()");
        exit(1);
    }

    //映射
    char* str = NULL;
    str = mmap(str, statres.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (str == MAP_FAILED) {
        perror("mmap()");
        exit(1);
    }
    
    int count = 0;
    for (int i = 0; i < statres.st_size; ++i) {
        if (str[i] == 'a') {
            count++;
        }
    }

    printf("%d\n", count);
    //解除映射
    munmap(str, statres.st_size);
    exit(0);
}