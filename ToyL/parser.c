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

/*判断是否类型说说明符*/
int IsType(int TkCode) {
	switch (TkCode) {
	case INT:
	case DOUBLE:
	case CHAR:
	case VOID:
		return TRUE;
	default:
		return FALSE;
	}
}

/*
<语法分析> -> {<全局声明>}<文件结束符>
*/
void Parser() {
	NextToken();
	while (token->TkCode != EOF) {
		Declaration();
	} 
}

/*
<声明> -> <类型声明><声明符>( <函数体>
							 | ["="<赋值表达式>]{","<声明符>["="<赋值表达式>]}";" )

<类型声明> -> "void" | "char" | "int" | "double"
<声明符> -> "标识符"<声明符后缀>
<声明符后缀> -> { "(" ")"
				| "(" <形参列表> ")"
				| "[" "]" 
				| "[" "整型常量" "]" }
<函数体> -> <复合语句>
*/
void Declaration() {
	TypeState();
	Declarator();
	while (TRUE) {
		if (token->TkCode == L_BRACE) {
			FuncBody();
			break;
		}
		else {
			if (token->TkCode == ASSIGN) {
				NextToken();
				AssignExpr();
			}
			else if (token->TkCode == COMMA) {
				NextToken();
				Declarator();
			}
			else {
				Skip(SEMI);
				break;
			}
		}

	}
}

/*
<类型声明> -> "void" | "char" | "int" | "double"
*/
void TypeState(){
	switch (token->TkCode) {
	case CHAR:
	case VOID:
	case INT:
	case DOUBLE:
		NextToken();
		break;
	default:
		Error("C_error: Absence type statement!");
		break;
	}
}

/*
<声明符> -> "标识符"<声明符后缀>
*/
void Declarator() {
	if (token->TkCode < IDENT) {
		Error("C_error: Absence identifier!");
	}
	else {
		NextToken();
	}
	DeclarationSuffix();
}

/*
<声明符后缀> -> { "(" ")"
				| "(" <形参列表> ")"
				| "[" "]"
				| "[" "整型常量" "]" }
*/
void DeclarationSuffix() {
	if (token->TkCode == L_PAREN) {
		FormalParaList();
		Skip(R_PAREN);
	}
	else if (token->TkCode == L_BRACK) {
		NextToken();
		if (token->TkCode == C_INT) {
			NextToken();
		}
		Skip(R_BRACK);
		DeclarationSuffix();
	}
}

/*
<形参列表>-><类型说明符>[<声明符>]{","<类型说明符><声明符>}
*/
void FormalParaList() {
	NextToken();
	while (token->TkCode != R_PAREN) {
		TypeState();
		Declarator();
		NextToken();
		if (token->TkCode == R_PAREN) {
			break;
		}
		else {
			Skip(COMMA);
		}
	}
}

/*
<函数体> -> <复合语句>
*/
void FuncBody() {
	ComplexStatement();
	Skip(R_BRACE);
}

/*
<语句> -> <复合语句>
		 |<条件语句>
		 |<循环语句>
		 |<return语句>
*/
void Statement() {
	switch (token->TkCode) {
	case L_BRACE:
		ComplexStatement();
		break;
	case IF:
		ConditionStatement();
		break;
	case FOR:
		LoopStatement();
		break;
	case  RETURN:
		ReturnStatement();
		break;
	default:
		break;
	}
}

/*
<复合语句> -> {{<声明>}{<语句>}}
*/
void ComplexStatement(){
	NextToken();
	while (IsType(token->TkCode)) {
		Declaration();
	}
	while (token->TkCode != R_BRACE) {
		Statement();
	}
}

/*
<条件语句> -> "if""("<表达式>")"<语句>["else"<语句>]
*/
void ConditionStatement() {
	NextToken();
	Skip(L_PAREN);
	Expression();
	Skip(R_PAREN);
	Statement();
	if (token->TkCode == ELSE) {
		NextToken();
		Statement();
	}
}

/*
<循环语句> -> "for""("<表达式>";"<表达式>";"<表达式>")"<语句>
*/
void LoopStatement() {
	NextToken();
	Skip(L_PAREN);
	Expression();
	Skip(SEMI);
	Expression();
	Skip(SEMI);
	Expression();
	Skip(R_PAREN);
	Statement();
}

/*
<return语句> -> "return" [<表达式>] ";"
*/
void ReturnStatement() {
	NextToken();
	if (token->TkCode != SEMI) {
		Expression();
		Skip(SEMI);
	}
	else {
		Skip(SEMI);
	}
}

/*
<表达式> -> <赋值表达式>{","<赋值表达式>}
*/
void Expression() {
	while (TRUE) {
		AssignExpr();
		if (token->TkCode == COMMA) {
			AssignExpr();
		}
		else {
			break;
		}
	}
}

/*
<赋值表达式> -> <逻辑或表达式>["="<赋值表达式>]
*/
void AssignExpr() {
	LogicOrExpr();
	if (token->TkCode == ASSIGN) {
		NextToken();
		AssignExpr();		
	}
}

/*
<逻辑或表达式> -> <逻辑与表达式>{"||"<逻辑与表达式>}
*/
void LogicOrExpr() {
	LogicAndExpr();
	while (TRUE) {
		if (token->TkCode == OR) {
			NextToken();
			LogicAndExpr();
		}
		else {
			break;
		}
	}
}

/*
<逻辑与表达式> -> <相等表达式>{"&&"<相等表达式>}
*/
void LogicAndExpr() {
	EqualExpr();
	while (TRUE) {
		if (token->TkCode == AND) {
			NextToken();
			EqualExpr();
		}
		else {
			break;
		}
	}
}

/*
<相等表达式> -> <关系表达式>{("=="|"!=")<关系表达式>}
*/
void EqualExpr() {
	RelationExpr();
	while (TRUE) {
		if (token->TkCode == EQ || token->TkCode == NEQ) {
			NextToken();
			RelationExpr();
		}
		else {
			break;
		}
	}
}

/*
<关系表达式> -> <加减表达式>{("<"|">"|"<="|">=")<加减表达式>}
*/
void RelationExpr() {
	AddMinusExpr();
	while (TRUE) {
		if (token->TkCode == LT || token->TkCode == GT
			|| token->TkCode == LE || token->TkCode == GE) {
			NextToken();
			AddMinusExpr();
		}
		else {
			break;
		}
	}
}

/*
<加减表达式> -> <乘除表达式>{("+"|"-")<乘除表达式>}
*/
void AddMinusExpr() {
	MultiDivideExpr();
	while (TRUE) {
		if (token->TkCode == PLUS || token->TkCode == MINUS) {
			NextToken();
			MultiDivideExpr();
		}
		else {
			break;
		}
	}
}

/*
<乘除表达式> -> <基本表达式>{("*"|"/")<基本表达式>}
*/
void MultiDivideExpr() {
	BaseExpr();
	while (TRUE) {
		if (token->TkCode == MULTI || token->TkCode == DIVIDE) {
			NextToken();
			BaseExpr();
		}
		else {
			break;
		}
	}
}

/*
<基本表达式> -> <元表达式>{"["<表达式>"]"
						  |"("[<表达式>]")"}
*/
void BaseExpr() {
	ElementExpr();
	while (TRUE) {
		if (token->TkCode == L_BRACK) {
			Expression();
			Skip(R_BRACK);
		}
		else if (token->TkCode == L_PAREN) {
			NextToken();
			if (token->TkCode != R_PAREN) {
				Expression();
				Skip(R_PAREN);
			}
			else{
				Skip(R_PAREN);
			}
		}
		else {
			break; 
		}
	}
}

/*
<元表达式> -> "标识符" | "整型常量" | "浮点型常量" | "字符常量" | "字符串常量"
			| "("<表达式> ")"
*/
void ElementExpr() {
	switch (token->TkCode) {
	case IDENT:
	case C_INT:
	case C_DOUBLE:
	case C_CHAR:
	case C_STR:
		NextToken();
		break;
	case L_PAREN:
		NextToken();
		Expression();
		Skip(R_PAREN);
		break;
	default:
		Error("缺少标识符或常量!");
	}
}
