#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
	if (argc != 2){
		fprintf(stderr,"引数の数が正しくありません");
		return -1;
	}

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");
	printf("\tmov rax, %d\n",atoi(argv[1]));
	printf("\tret\n");
	return 0;
}
