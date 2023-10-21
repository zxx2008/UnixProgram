#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	FILE* fp;
	fp = fopen("tmp", "r");
	if (fp == NULL) {
		fprintf(stderr, "fopen() failed! errno = %d\n", errno);
		exit(1);
	}
	puts("OK!");
    fclose(fp);
	exit(0);
}
