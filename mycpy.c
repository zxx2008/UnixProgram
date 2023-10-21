#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: %s <src_file> <dest_file>\n", argv[0]);
		exit(1);
	}	

	FILE *fps, *fpd;
	int ch;
	fps = fopen(argv[1], "r");
	if (fps == NULL) {
		fclose(fps);
		perror("fopen()");
	}
	fpd = fopen(argv[2], "w");
	if (fpd == NULL) {
		fclose(fps);
		fclose(fpd);
		perror("fopen()");
	} 
	while(1) {
		ch = fgetc(fps);
		if (ch == EOF) break;
		fputc(ch, fpd);
	}
	fclose(fpd);
	fclose(fps);
	
	exit(0);
}
