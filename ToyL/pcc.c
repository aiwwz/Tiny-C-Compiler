#include <stdio.h>
#include "pcc.h"
FILE *FP;

int main(int argc, char *argv[]) {
	FP = fopen("test.txt", "r");
	if (!FP) {
		Error("Open file fail!");
	}
	InitLex();
	Next_token();
	while (token->TkCode != EOF) {
		Next_token();
	}
	printf("Parse success!\n");
	return 0;
}