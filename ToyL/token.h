#ifndef  TOKEN_H
#define TOKEN_H

typedef struct Token_Node * PtrToNode;
typedef PtrToNode Token;
typedef PtrToNode Position;

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
