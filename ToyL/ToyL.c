#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "expr_val.h"

//Token--单词的编码
//已经内置定义 EOF (-1)
#define BEGIN 0
#define NUMB 1
#define IDEN 2
#define PLUS 3
#define MULT 4
#define ASS 5 //定义‘:=’赋值的宏名
#define READ 6
#define WRITE 7
#define SEMI 8
#define OPEN 9
#define CLOSE 10
#define END 11


//判断读入的字符的类型
#define is_end_of_input(ch) ((ch)== -1)
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
/*
struct Token_Node {
int _class;  //单词编码
char * seman;   //语义
Position  last;
Position  next;
};
*/
char ch;  //定义全局字符变量
FILE *fp;  //定义全局文件指针变量
Position P;  //定义全局token指针
Position P_old; //用于语法分析过程中的单词回溯

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

void back() {
	fseek(fp, -1, 1); //将文件指针从当前位置向前移动一个字节
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
	Tmp->_class = NUMB;
	Tmp->seman = malloc(sizeof(char) + 1);
	Tmp->seman[0] = N;  //将翻译后的数字直接存入字符数组中，字符数组的第一个元素即为ASCII码为N对应的字符
	Tmp->seman[1] = '\0';
	P->next = Tmp;
	Tmp->last = P;
	P = Tmp;
	back();
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
		Tmp->_class = BEGIN;
		Tmp->seman = "begin";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "end") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = END;
		Tmp->seman = "end";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		return; //end保留字不能使用back()函数，应当直接返回；
	}
	else if (strcmp(name, "read") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = READ;
		Tmp->seman = "read";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "write") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = WRITE;
		Tmp->seman = "write";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = IDEN;
		Tmp->seman = malloc(sizeof(char) * strlen(name));
		strcpy(Tmp->seman, name);
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	back();
}

//仅读一个单词的词法分析器
void next_token(void) {

	next_avail_char();
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
		Tmp->_class = PLUS;
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
		Tmp->_class = MULT;
		Tmp->seman = "*";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ':': {
		next_Char();
		if (ch != '=') Error("Error: invalid identifier!\n");
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = ASS;
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
		Tmp->_class = SEMI;
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
		Tmp->_class = OPEN;
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
		Tmp->_class = CLOSE;
		Tmp->seman = ")";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case  EOF: {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = EOF;
		Tmp->seman = "eof";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	default: break;
	}
}

//语法分析器部分
void token(int _class) {
	if (P->_class != _class) {
		Error("Syntax Error!\n");
	}
	P = P->next;
}

void expr_parser() {
loop:	if (P->_class != IDEN && P->_class != NUMB) {
	Error("Syntax Error!\n");
}
		P = P->next;
		if (P->_class == PLUS || P->_class == MULT) {
			P = P->next;
			goto loop;
		}
		return;
}

void prog_parser() {
	token(BEGIN);
	while (P->_class != END) {
		switch (P->_class) {
		case READ: P = P->next; token(OPEN); token(IDEN); token(CLOSE); token(SEMI); break;
		case WRITE: P = P->next; token(OPEN); expr_parser(); token(CLOSE); token(SEMI); break;
		case IDEN: P = P->next; token(ASS); expr_parser(); token(SEMI); break;
		default: Error("Syntax Error!\n"); break;
		}
	}
	P = P->next;
	token(EOF);

	return;
}

//代码解释模块

//表达式求值


/*
//输入输出
void in_out() {

}
*/

void interpreter() {
	token(BEGIN);
	Position Tmp;
	while (P->_class != END) {
		switch (P->_class) {
		case READ:
			P = P->next;
			token(OPEN);
			updata(P->seman, read());
			P = P->next;
			token(CLOSE);
			token(SEMI);
			break;
		case WRITE:
			P = P->next;
			token(OPEN);
			printf("%d\n", fetch(P->seman));
			P = P->next;
			token(CLOSE);
			token(SEMI);
			break;
		case IDEN:
			Tmp = P;
			P = P->next;
			token(ASS);
			updata(Tmp->seman, expr_val(P));
			P = P->next;
			if (mp == 2) {
				P = P->next;
				P = P->next;
			}
			token(SEMI);
			break;
		default: Error("Syntax Error444!\n");
		}
	}
}

int main() {
	Token T;
	T = init_token();
	char c[100];
	printf("Input directory of file:");
	gets(c);
	fp = fopen(c, "r");

	//词法分析
	while (P->_class != EOF) {
		next_token();
	}
	P = T->next;

	//语法分析
	prog_parser();

	//代码解释
	P = T->next;
	interpreter();

	fclose(fp);
	return 0;
}