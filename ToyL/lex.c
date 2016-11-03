#include "pcc.h"
#include <stdio.h>
#include <ctype.h>

/******** 定义全局变量 ********/
Vector TokenTable;		//单词表
HashTable TokenHash;	//哈希表
String Str;				//字符串
Token token;			//单词
FILE *FP;
char ch;

/* lex初始化 */
void InitLex() {
	TokenTable = InitVector(TKSIZE);
	TokenHash = InitHash(HASHSIZE);
	Str = InitString(STRSIZE);
}

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
				RecognizeAnnot();
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
	token = VectorAdd(TokenTable, TokenHash, Str);
	token->TkCode = IDENT;
}

/* 解析常量--字符、字符串 */
void RecognizeConst() {

}