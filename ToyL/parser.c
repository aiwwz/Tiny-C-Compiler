#include "pcc.h";

/*跳过当前单词*/
void Skip(int TkCode) {
	if (token->TkCode == TkCode) {
		NextToken();
		return;
	}
	else {
		printf("Absence %s\n", Token_Table->Data[TkCode]);
	}
}

/*
<分析单元> -> {<全局声明>}<文件结束符>
*/
void Parser_Unit() {
	while (token->TkCode != EOF) {
		Extern_Declaration();
	}
}

/*
<全局声明> -> <类型声明>(  ";"
						  | <声明符><函数体>
						  | <声明符>["="<赋值表达式>]{","<声明符>["="<赋值表达式>]}";" )

<类型声明> -> "void" | "char" | "int" | "double"
<声明符> -> <标识符><声明符后缀>
<声明符后缀> -> { "(" ")"
				| "[" "]"
				| "(" <形参列表> ")"
				| "[" "整型常量" "]" }
<函数体> -> <复合语句>
*/
void Extern_Decalration() {
	if (!TypeState()) {
		Error("Absence type statement!");
	}
	if (token->TkCode == SEMI) {
		NextToken();
		return;
	}
	while (TRUE) {
		Declaration();
		if (token->TkCode == L_BRACE) {
			FunBody();
		}
		else {
			if (token->TkCode == ASSIGN) {
				AssignExpr();
			}
			else if (token->TkCode == COMMA) {
				NextToken();
			}
			else {
				Skip(SEMI);
				return;
			}
		}
	}
}

/*
<类型声明> -> "void" | "char" | "int" | "double"
*/
int TypeState(){
	switch (token->TkCode) {
	case CHAR:
	case VOID:
	case INT:
	case DOUBLE:
		NextToken();
		return TRUE;
	default:
		return FALSE;
	}
}