//实现删除文件的第10行
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: ./deletLine <file> line_number");
		exit(1);
	}

	int fd1 = open(argv[1], O_RDONLY);
	
	if (fd1 < 0) {
		perror("open()");
		exit(1);
	}
	int fd2 = open(argv[1], O_RDWR);
	
	if (fd2 < 0) {
		perror("open()");
		exit(1);
	}
	
	
	int pos = 0, row = 0;
	char str[2];
	//找到写入的位置
	while (1) {
		if (row == 9) {
			break;
		}	
		if (read(fd2, str, 1) <= 0) {
			break;
			exit(1);
		} 
		if (str[0] == '\n') {
			++row;
		}
		++pos;
	}
		
	printf("%d\n", row);
	printf("%d\n", pos);
	
	//exit(0);	
	int offset = lseek(fd1, pos, SEEK_SET);
	printf("%d\n", offset);
	
	while (1) {
		if (row == 10) {
			break;
		}
		if (read(fd1, str, 1) <= 0) {
			break;
			exit(1);
		}
		if (str[0] == '\n') {
			++row;
		}
		++pos;
	}

	while (1) {
		if (read(fd1, str, 1) <= 0) break;
		write(fd2, str, 1);
	}	
	int trunc_num = lseek(fd2, 0, SEEK_CUR);
	ftruncate(fd2, trunc_num);
	close(fd1);
	close(fd2);
	exit(0);
}

