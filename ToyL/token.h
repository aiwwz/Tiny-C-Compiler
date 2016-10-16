#ifndef  TOKEN_H
#define TOKEN_H

typedef struct Token_Node * PtrToNode;
typedef PtrToNode Token;
typedef PtrToNode Position;

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

struct Token_Node
{
	int _class;
	char * seman;
	Position last;
	Position next;
};

Token init_token();

//两种类型的读字符函数：过滤格式符
void next_Char();
void next_avail_char();


#endif // ! TOKEN_H
