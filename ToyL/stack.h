#pragma once
#ifndef STACK_H;
#define STACK_H;

#include <stdio.h>
#include <stdlib.h>
#define MinStackSize (5)
#define EmptyOfStack (-1)
#define FatalError(STR) printf("%s",STR), exit(-1)
#define Error(STR) FatalError(STR)
#define PrintError(STR) printf("%s\n", STR)

typedef double ElementType;
typedef struct StackRecord * Stack;

struct StackRecord {
	int TopOfStack;
	int Capacity;
	ElementType *Array;
};

Stack InitStack(int MaxStackSize) {
	Stack S;
	if (MaxStackSize < MinStackSize)
		Error("Stack size is too small!");
	S = (Stack)malloc(sizeof(struct StackRecord));
	if (S == NULL)
		FatalError("Out of space!!!");
	S->Array = (ElementType *)malloc(sizeof(ElementType) * MaxStackSize);
	if (S->Array == NULL)
		FatalError("Out of space!!!");
	S->Capacity = MaxStackSize;
	ClearStack(S);
	return S;
}
void DestroyStack(Stack S) {
	if (S != NULL) {
		free(S->Array);
		free(S);
	}
}
int IsEmpty(Stack S) {
	return S->TopOfStack == EmptyOfStack;
}
ElementType Top(Stack S) {
	if (!IsEmpty(S))
		return S->Array[S->TopOfStack];
	else
		Error("Empty stack!!1!");
}
void Push(Stack S, ElementType E) {
	S->Array[++S->TopOfStack] = E;
}
void Pop(Stack S) {
	if (IsEmpty(S))
		Error("Empty Stack!!!");
	else
		--S->TopOfStack;
}
ElementType TopAndPop(Stack S) {
	if (!IsEmpty(S))
		return S->Array[--S->TopOfStack];
	else
		Error("Empty stack!!!");
	return 0;
}


#endif // !STACK_H;

