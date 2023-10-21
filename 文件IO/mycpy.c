/*
 *use system IO function to make a copy 
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage ./mycpy <src_file> <des_file>\n");
		exit(1);
	}

	int sfd, dfd;
	int pos = 0;

	sfd = open(argv[1], O_RDONLY);
	if (sfd < 0) {
		perror("open()");
		exit(1);
	}

	dfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dfd < 0) {
		perror("open()");
		exit(1);
	}
	
	
	int rlen, wlen;
	char buf[BUFSIZE];

	while (1) {
		
		rlen = read(sfd, buf, BUFSIZE);
		if (rlen < 0) {
			perror("read()");
			break;
		}
		if (rlen == 0) {
			break;
		}
		pos = 0;
		while (rlen > 0) {
			wlen = write(dfd, buf + pos, rlen);
			if (wlen < 0) {
				perror("write()");
				exit(1);
			}
			pos += wlen;
			rlen -= wlen;
		}
	}

	close(dfd);
	close(sfd);
	exit(0);
}
