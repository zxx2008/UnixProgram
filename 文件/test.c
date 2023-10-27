#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define PATH "/mnt/d/workspace/js"

int main() {
    struct stat statbuf;
    lstat(PATH, &statbuf);
    printf("%ld\n", statbuf.st_blocks);
    exit(0);
}