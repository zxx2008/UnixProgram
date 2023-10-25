//获得文件的大小
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//该函数返回文件大小
static off_t flen(const char *pathname) {
	struct stat statbuf;
	
	if (stat(pathname, &statbuf) < 0) {
		perror("stat()");
		exit(1);
	}
	
	return statbuf.st_size;
}


int main(int argc, char **argv) {
	
	if (argc < 2) {
		fprintf(stderr, "Usage: ./flen <file>\n");
	}

	fprintf(stdout, "%lld\n", (long long)flen(argv[1]));
}
