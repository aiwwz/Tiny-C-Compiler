#include "toyc.h";

/*跳过当前单词*/
void Skip(int TkCode) {
	if (token->TkCode == TkCode) {
		NextToken();
		return;
	}
	else {
		printf("C_error:缺少%s\n", Token_Table->Data[TkCode]->String);
		exit(-1);
	}
}

/*判断是否类型说说明符*/
int IsType(int TkCode) {
	switch (TkCode) {
	case CHAR:
	case STRING:
	case VOID:
	case INT:
	case DOUBLE:
		return TRUE;
	default:
		return FALSE;
	}
}

/*
<语法分析> -> {<声明>}<文件结束符>
*/
void Parser() {
	NextToken();
	while (token->TkCode != _EOF) {
		Declaration();
	} 
}

/*
<声明> -> <头文件包含>
		 | <类型声明><声明符>( <函数体>
							 | ["="<赋值表达式>]{","<声明符>["="<赋值表达式>]}";" )
*/
void Declaration() {
	if (token->TkCode == INCLUDE) {
		IncludeHeader();
	}
	else if (IsType(token->TkCode)) {
		TypeState();
		Declarator(); 
		if (token->TkCode == L_BRACE) {
			FuncBody();
		}
		else {
				if (token->TkCode == ASSIGN) {
					NextToken();
					AssignExpr();
				}
				while (token->TkCode == COMMA) {
					NextToken();
					Declarator();
					if (token->TkCode == ASSIGN) {
						NextToken();
						AssignExpr();
					}
				}
				Skip(SEMI);
		}
	}
}

/*
<头文件包含> -> "#include" "头文件常量"
*/
void IncludeHeader() {
	NextToken();
	Skip(C_HEADER);
}

/*
<类型声明> -> "void" | "char" | "int" | "double"
*/
void TypeState(){
	switch (token->TkCode) {
	case CHAR:
	case STRING:
	case VOID:
	case INT:
	case DOUBLE:
		NextToken();
		break;
	default:
		Error("C_error:缺少类型说明符!");
		break;
	}
}

/*
<声明符> -> "标识符"<声明符后缀>
*/
void Declarator() {
	Token tokenTmp = token;
	if (token->TkCode < IDENT) {
		Error("C_error:缺少标识符!");
	}
	NextToken();
	if (token->TkCode == L_PAREN) {
		tokenTmp->TkCode = FUNC;
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
}

/*
<语句> -> <复合语句>
		 |<函数调用语句>
		 |<if条件语句>
		 |<switch条件语句>
		 |<for循环语句>
		 |<while循环语句>
		 |<case语句>
		 |<default语句>
		 |<break语句>
		 |<return语句>
		 |<表达式语句>
*/
void Statement() {
	switch (token->TkCode) {
	case L_BRACE:
		ComplexStatement();
		break;
	case FUNC:
		FuncCall();
		break;
	case IF:
		IfStatement();
		break;
	case SWITCH:
		SwitchStatement();
		break;
	case FOR:
		ForStatement();
		break;
	case WHILE:
		WhileStatement();
		break;
	case CASE:
		CaseStatement();
		break;
	case DEFAULT:
		DefaultStatement();
		break;
	case BREAK:
		BreakStatement();
		break;
	case  RETURN:
		ReturnStatement();
		break;
	default:
		ExprStatement();
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
	Skip(R_BRACE);
}

/*
<函数调用语句> -> "函数名""("[<表达式>]")"
*/
void FuncCall() {
	NextToken();
	Skip(L_PAREN);
	if (token->TkCode != R_PAREN) {
		Expression();
	}
	Skip(R_PAREN);
	Skip(SEMI);
}

/*
<if条件语句> -> "if""("<表达式>")"<语句>["else"<语句>]
*/
void IfStatement() {
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
<switch条件语句> -> "switch""("<表达式>")"<语句>
*/
void SwitchStatement() {
	NextToken();
	Skip(L_PAREN);
	Expression();
	Skip(R_PAREN);
	Statement();
}

/*
<for循环语句> -> "for""("([<声明>]|[<表达式>";"])[<表达式>]";"[<表达式>]")"<语句>
*/
void ForStatement() {
	NextToken();
	Skip(L_PAREN);
	if (IsType(token->TkCode)) {
		Declaration();
	}
	else {
		Expression();
		Skip(SEMI);
	}
	if (token->TkCode != SEMI) {
		Expression();
	}
	Skip(SEMI);
	if (token->TkCode != R_PAREN) {
		Expression();
	}
	Skip(R_PAREN);
	Statement();
}

/*
<while循环语句> -> "while""("<表达式>")"<语句> 
*/
void WhileStatement() {
	NextToken();
	Skip(L_PAREN);
	Expression();
	Skip(R_PAREN);
	Statement();
}

/*
<case语句> -> "case"("整型常量"|"字符型常量")":"
*/
void CaseStatement() {
	NextToken();
	if (token->TkCode == C_INT || token->TkCode == C_CHAR) {
		NextToken();
	}
	else {
		Error("C_error:缺少整型常量表达式!");
	}
	Skip(COLON);
}

/*
<default语句> -> "default"":"
*/
void DefaultStatement() {
	NextToken();
	Skip(COLON);
}

/*
<break语句> -> "break"";"
*/
void BreakStatement() {
	NextToken();
	Skip(SEMI);
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
<表达式语句> -> <表达式>";"
*/
void ExprStatement() {
	Expression();
	Skip(SEMI);
}

/*
<表达式> -> <赋值表达式>{","<赋值表达式>}
*/
void Expression() {
		AssignExpr();
		while (token->TkCode == COMMA) {
			NextToken();
			AssignExpr();
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
	while (token->TkCode == OR) {
		NextToken();
		LogicAndExpr();
	}
}

/*
<逻辑与表达式> -> <相等表达式>{"&&"<相等表达式>}
*/
void LogicAndExpr() {
	EqualExpr();
	while (token->TkCode == AND) {
		NextToken();
		EqualExpr();
	}
}

/*
<相等表达式> -> <关系表达式>{("=="|"!=")<关系表达式>}
*/
void EqualExpr() {
	RelationExpr();
	while (token->TkCode == EQ || token->TkCode == NEQ) {
		NextToken();
		RelationExpr();
	}
}

/*
<关系表达式> -> <加减表达式>{("<"|">"|"<="|">=")<加减表达式>}
*/
void RelationExpr() {
	AddMinusExpr();
	while (token->TkCode == LT || token->TkCode == GT || token->TkCode == LE || token->TkCode == GE) {
		NextToken();
		AddMinusExpr();
	}
}

/*
<加减表达式> -> <乘除表达式>{("+"|"-")<乘除表达式>}
*/
void AddMinusExpr() {
	MultiDivideExpr();
	while (token->TkCode == PLUS || token->TkCode == MINUS) {
		NextToken();
		MultiDivideExpr();
	}
}

/*
<乘除表达式> -> <基本表达式>{("*"|"/"|"%")<基本表达式>}
*/
void MultiDivideExpr() {
	BaseExpr();
	while (token->TkCode == MULTI || token->TkCode == DIVIDE || token->TkCode == MOD) {
		NextToken();
		BaseExpr();
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
			NextToken();
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
		if (token->TkCode >= IDENT) {
			NextToken();
			break;
		}
		else {
			Error("C_error:缺少标识符或常量!");
		}
	}
}
