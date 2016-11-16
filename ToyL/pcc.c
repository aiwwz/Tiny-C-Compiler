#include <stdio.h>
#include "pcc.h"
FILE *FP;

int main(int argc, char *argv[]) {
	FP = fopen("test.txt", "r");
	if (!FP) {
		Error("Open file fail!");
	}

	InitLex();
	/*
	NextToken();
	while (token->TkCode != _EOF) {
		NextToken();
	}
	*/
	Parser();
	printf("Parse success!\n");
	return 0;
}