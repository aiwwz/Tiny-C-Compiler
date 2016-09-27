#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Token--单词的编码
#define BEGIN 1
#define NUMB 2
#define IDEN 3
#define PLUS 4
#define MULT 5
#define ASS 6 //定义‘:=’赋值的宏名
#define READ 7
#define WRITE 8
#define SEMI 9
#define OPEN 10
#define CLOSE 11
#define END 12


//判断读入的字符的类型
#define is_end_of_input(ch) ((ch)== '\0')
#define is_lc_letter(ch) ('a'<=(ch) && (ch)<='Z')
#define is_uc_letter(ch) ('A'<=(ch) && (ch)<='Z')
#define is_letter(ch) ('a'<=(ch) && (ch)<='z' || 'A'<=(ch) && (ch)<='Z')
#define is_digit(ch) ('0'<=(ch) && (ch)<='9')
#define is_letter_or_digit(ch) (is_letter(ch) && is_digit(ch))
#define is_operator(ch) ((ch)=='+' || (ch)=='-' || (ch)=='*')
#define is_layout(ch) (!is_end_of_input(ch) && (ch)<=' ')  //格式符

typedef struct Token_Node * PtrToNode;
typedef PtrToNode Token;
typedef PtrToNode Position;

struct Token_Node {
	int class;  //单词编码
	char * seman;   //语义
	Position  last;
	Position  next;
};

char ch;  //定义全局字符变量
FILE *fp;  //定义全局文件指针变量
Position P;  //定义全局token指针

Token init_token() {
	Token T;
	T = malloc(sizeof(struct Token_Node));
	T->next = NULL;
	T->last = NULL;
	P = T;
	return T;
}

//两种类型的读字符函数：过滤格式符
void next_Char() {
	ch = fgetc(fp);
}
void next_avail_char() {
	next_Char();
	while (is_layout(ch)) {
		next_Char();
	}
}

//读进常数并将常数从字符串翻译到数字
void recognize_number() {
	int N = 0;
	while (is_digit(ch)) {
		N = N * 10 + (ch)-'0';
		next_Char();
	}
	Position Tmp;
	Tmp = malloc(sizeof(struct Token_Node));
	Tmp->class = NUMB;
	Tmp->seman = malloc(sizeof(char) + 1);
	Tmp->seman[0] = N;  //将翻译后的数字直接存入字符数组中，字符数组的第一个元素即为ASCII码为N对应的字符
	Tmp->seman[1] = '\0';
	P->next = Tmp;
	Tmp->last = P;
	P = Tmp;
	//back();
}

//读进标识符
void recognize_name() {
	char name[10];
	int np = 0;
	name[np++] = ch;
	next_Char();
	while (is_digit(ch) || is_letter(ch)) {
		name[np++] = ch;
		next_Char();
	}
	name[np] = '\0';
	if (strcmp(name, "begin") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = BEGIN;
		Tmp->seman = "begin";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "end") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = END;
		Tmp->seman = "end";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "read") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = READ;
		Tmp->seman = "read";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "write") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = WRITE;
		Tmp->seman = "write";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = IDEN;
		Tmp->seman = malloc(sizeof(char) * strlen(name));
		strcpy(Tmp->seman, name);
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	//back();
}

//仅读一个单词的词法分析器
void next_token(void) {
	next_avail_char();
	printf("1\n");
	switch (ch) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': recognize_number(); break;

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
	case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
	case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
	case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
	case 'W': case 'X': case 'Y': case 'Z': recognize_name(); break;

	case '+': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = PLUS;
		Tmp->seman = "+";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL; 
		break;
	}
	case '*': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = MULT;
		Tmp->seman = "*";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ':': {
		next_Char(); 
		if (ch != '=') break;
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = ASS;
		Tmp->seman = ":=";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ';': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = SEMI;
		Tmp->seman = ";";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case '(': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = OPEN;
		Tmp->seman = "(";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ')': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = CLOSE;
		Tmp->seman = ")";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case '\0': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->class = EOF;
		Tmp->seman = "eof";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case EOF: printf("eof"); break;
	default: break;
	}
}


int main() {
	Token T;
	T = init_token();
	fp = fopen("C:\\1.txt", "r");

	do {
		next_token();
	} while (ch != EOF);

	P = T->next;
	while (P != NULL) {
		printf("%d,%s\n", P->class, P->seman);
		P = P->next;
	}

	return 0;
}