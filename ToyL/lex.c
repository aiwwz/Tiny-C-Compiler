#include "pcc.h"
#include <stdio.h>
#include <stdlib.h>

/******** 定义全局变量 ********/
Vector Token_Table;		//单词表
HashTable Hash_Table;	//哈希表
String Str;				//字符串
Token token;			//单词
FILE *FP;
char ch;

/* 读字符 */ 
void NextChar() {
	ch = fgetc(FP);
}

/* 带过滤的读字符 */
void NextAvailChar() {
	NextChar();
	while (1){
		if (IsLayout(ch)) {
			NextChar();
		}
		else if (ch == '\/') {
			NextChar(); //多读一个判断是不是注释
			if (ch == '*') {
				RecognizeNote();
			}
			else { //将多读的写回stream
				if (ungetc(ch, FP) == EOF) {
					Error("Write back error!");
				}
				ch = '\/';
				break;
			}
		}
		else
			break;
	}
}

/* 解析标识符 */
void RecognizeIden() {
	StringReset(Str);
	while (IsLetter(ch) || IsDigit(ch) || IsUnderline(ch)) {
		StringAdd(Str, ch);
		NextChar();
	}
	token = VectorAdd(Token_Table, Hash_Table, Str);
	token->TkCode = IDENT;
}

/* 解析数字常量 */
void RecognizeNum() {
	StringReset(Str);
	int M, cnt = 0;
	double N;
	while (IsDigit(ch) || IsDot(ch)) {
		if (IsDot(ch)) {
			cnt++;
		}
		StringAdd(Str, ch);
		NextChar();
	}
	if (cnt == 0) { //整型常量
		token = VectorAdd(Token_Table, Hash_Table, Str);
		token->TkCode = C_INT;
	}
	else if (cnt == 1) { //浮点型常量
		token = VectorAdd(Token_Table, Hash_Table, Str);
		token->TkCode = C_DOUBLE;
	}
	else {
		Error("词法错误！");
	}
}

/* 解析常量--字符、字符串 */
void RecognizeConst(){
	StringReset(Str);
	char c = ch;
	while (1) {
		NextChar();
		if (ch == c) {
			break;
		}
		else {
			StringAdd(Str, ch);
			NextChar();
		}
	}
	if (c == '\'') {
		token = VectorAdd(Token_Table, Hash_Table, Str);
		token->TkCode = C_CHAR;
	}
	else {
		token = VectorAdd(Token_Table, Hash_Table, Str);
		token->TkCode = C_STR;
	}
}


/* 解析注释 */
void RecognizeNote() {
	while (1) {
		NextChar();
		if (ch == '*') {
			NextChar();
			if (ch == '\/') {
				break;
			}
		}
	}
}

/* lex初始化 */
void InitLex() {
	Token_Table = InitVector(TKSIZE);
	Hash_Table = InitHash(HASHSIZE);
	Str = InitString(STRSIZE);
	Token_Table->Data[]
}