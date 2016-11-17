#ifndef PCC_H
#define PCC_H
#include <stdio.h>
#define FatalError(Str) printf("%s\n", Str), exit(-1)
#define Error(Str) FatalError(Str)
#define TRUE 1
#define FALSE 0
#define TKSIZE 1024
#define STRSIZE 20
#define HASHSIZE 1000

/********************** lex.c **********************/
#define IsLetter(ch) ('a'<= ch && ch <='z' || 'A'<= ch && ch <= 'Z')
#define IsDigit(ch) ('0'<= ch && ch <='9')
#define Is_Layout(ch) (ch != EOF && ch <= ' ')
#define IsUnderline(ch) (ch == '_')
#define IsDot(ch) (ch == '.')
typedef struct TokenWord *PtrToToken;
typedef PtrToToken Position;
typedef PtrToToken Token;

/*单词存储结构*/
struct TokenWord {
	int TkCode;		//单词编码
	char *String;	//单词字符串
	Token Next;		//哈希冲突的下一个Token
};
void NextChar();
void NextAvailChar();
void RecognizeNote();
void NextAvailChar();
void InitLex();
void RecognizeInclude();
void RecognizeIden();
void RecognizeNum();
void RecognizeConst();
void RecognizeNote();
void NextToken();


enum TkCode {
	/*关键字*/
	BREAK,		//关键字 break
	CHAR,		//关键字 char
	CASE,		//关键字 case
	CONTINUE,	//关键字 continue
	DEFAULT,	//关键字 default
	DOUBLE,		//关键字 double
	ELSE,		//关键字 else
	FOR,		//关键字 for
	IF,			//关键字 if
	INCLUDE,	//关键字 #include
	INT,		//关键字 int
	RETURN,		//关键字 return
	STRING,		//关键字 string
	SWITCH,		//关键字 switch
	VOID,		//关键字 void
	WHILE,		//关键字 while

	/*运算符*/
	PLUS,		//加法运算符 '+'
	MINUS,		//减法运算符 '-'
	MULTI,		//乘法运算符 '*'
	DIVIDE,		//除法运算符 '/'
	MOD,		//取模运算符 '%'
	ASSIGN,		//赋值运算符 '='
	EQ,			//相等运算符 '=='
	NEQ,		//不相等运算符 '!='
	LT,			//小于运算符 '<'
	GT,			//大于运算符 '>'
	LE,			//小于等于运算符 '<='
	GE,			//大于等于运算符 '>='
	OR,			//或运算符 '||'
	AND,		//与运算符 '&&'

	/*分隔符*/
	L_PAREN,	//小括号 '('
	R_PAREN,	//小括号 ')'
	L_BRACK,	//中括号 '['
	R_BRACK,	//中括号 ']'
	L_BRACE,		//大括号 '{'
	R_BRACE,		//大括号 '}'
	COMMA,		//逗号 ','
	COLON,		//冒号 ':'
	SEMI,		//分号 ';'

	/*常量*/
	C_INT,		//整型常量		
	C_DOUBLE,	//浮点型常量
	C_CHAR,		//字符型常量
	C_STR,		//字符串常量
	C_HEADER,	//头文件常量
	
	/*文件结束符*/
	_EOF,

	/*标识符*/
	IDENT,		//标识符
	FUNC		//函数名
	
};


/******************** dynamic.c ********************/

/*动态字符串结构定义*/
#define MinStringSize 20
typedef struct DynamicString *String;
struct DynamicString {
	int Size;
	int Capacity;
	char *Data;
};
String InitString(int StringSize);
void StringAdd(char c);
void StringReset(String Str);

/*哈希表结构定义*/
#define MinTableSize 100
typedef struct HashTbl *HashTable;
struct HashTbl {
	int TableSize;
	Token *TheTokens;
};
int Hash(char *Key, HashTable H);
HashTable InitHash(int TableSize);
Position Find(char *Str);

/*动态数组结构定义*/
#define MinVectorSize 1000
typedef struct DynamicVector *Vector;
struct DynamicVector {
	int Size;
	int Capacity;
	Token *Data;
};
Vector InitVector(int VectorSize);
Token VectorAdd(String Str);
void VectorFree(Vector Vec);

/******************全局变量******************/
extern FILE *FP;
extern Token token;
extern Vector Token_Table;
extern HashTable Hash_Table;
extern String Str;

/******************** parser.c ********************/

void Skip(int TkCode);
int IsType(int TkCode);
void Parser();
void Declaration();
void IncludeHeader();
void TypeState();
void Declarator();
void DeclarationSuffix();
void FormalParaList();
void FuncBody();
void Statement();
void ComplexStatement();
void FuncCall();
void IfStatement();
void SwitchStatement();
void ForStatement();
void WhileStatement();
void CaseStatement();
void DefaultStatement();
void BreakStatement();
void ReturnStatement();
void ExprStatement();
void IntConstExpr();
void Expression();
void AssignExpr();
void LogicOrExpr();
void LogicAndExpr();
void EqualExpr();
void RelationExpr();
void AddMinusExpr();
void MultiDivideExpr();
void BaseExpr();
void ElementExpr();

#endif //PCC_H