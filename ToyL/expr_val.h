#ifndef EXPRE_VAL_H
#define EXPRE_VAL_H
#include "stack.h"
#include "token.h"
#include <string.h>
#include "Memory.h"

void Operation(Stack S_sym, Stack S_num) {  //后缀式计算
	int Tmp;
	if (Top(S_sym) == '+')
		Push(S_num, (TopAndPop(S_num) + TopAndPop(S_num)));
	if (Top(S_sym) == '*')
		Push(S_num, (TopAndPop(S_num) * TopAndPop(S_num)));
	if (Top(S_sym) == '-') {
		Tmp = TopAndPop(S_num);
		Push(S_num, (TopAndPop(S_num) - Tmp));
	}
	if (Top(S_sym) == '/') {
		Tmp = TopAndPop(S_num);
		Push(S_num, (TopAndPop(S_num) / Tmp));
	}
	Pop(S_sym);
}

int Priority(char a, char b) {  // 优先级判断
	int Pa, Pb;
	if (a == '+' || a == '-')
		Pa = 0;
	if (a == '*' || a == '/')
		Pa = 1;
	if (a == ')')
		Pa = -1;
	if (a == '(')
		Pa = 2;
	if (b == '+' || b == '-')
		Pb = 0;
	if (b == '*' || b == '/')
		Pb = 1;
	if (b == '(')
		Pb = -1;
	if (Pa == -1 && Pb == -1)
		return 0;
	else if (Pa <= Pb)
		return -1;
	else
		return 1;
}

int	 expr_val(Position P) {
	Stack S_sym, S_num;
	S_sym = InitStack(100);
	S_num = InitStack(100);
	int Result;
	for (; P->_class != SEMI; P = P->next) {
		if (P->_class == NUMB) {  //A[i] is number
			Push(S_num, (int)P->seman[0]);
		}
		else
			if (P->_class == IDEN) {
				Push(S_num, fetch(P->seman));
			}

			else
				if (P->_class == PLUS || P->_class == MULT || P->_class == OPEN) {  //A[i] is operator
					if (!IsEmpty(S_sym)) {
						if (Priority(P->seman[0], (char)Top(S_sym)) > 0)
							Push(S_sym, (int)P->seman[0]);
						else if (Priority(P->seman[0], (char)Top(S_sym)) < 0) {
							while (!IsEmpty(S_sym) && Priority(P->seman[0], (char)Top(S_sym)) < 0)
								Operation(S_sym, S_num);
							Push(S_sym, (int)P->seman[0]);
						}
					}
					else
						Push(S_sym, (int)P->seman[0]);
				}
				else
					if (P->_class == CLOSE) {
						while (Priority(P->seman[0], Top(S_sym)) != 0)
							Operation(S_sym, S_num);
						Pop(S_sym);
					}
					else {
						PrintError("Input Error!!!");
						return 0;
					}
	}

	while (!IsEmpty(S_sym))
		Operation(S_sym, S_num);
	Result = Top(S_num);
	DestroyStack(S_sym);
	DestroyStack(S_num);
	return Result;
}
#endif //  EXPRE_VAL_H

