/*getline用法*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	
	if(argc < 2) {
		fprintf(stdout, "Usage: ./getline <file>\n");	
		exit(1);
	}

	size_t lineSize = 0;
	char* linePtr = NULL;
	FILE* fp = fopen(argv[1], "r");
	while(getline(&linePtr, &lineSize, fp) >= 0);
	{
		fprintf(stdout, "%d\n", (int)strlen(linePtr));
		fprintf(stdout, "%d\n", (int)lineSize);
	}

	exit(0);

	


}
