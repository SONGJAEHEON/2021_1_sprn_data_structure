#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEBYZERO 3

FILE* fin;
FILE* fout;

typedef struct Stack{
	int* key;
	int top;
	int max_stack_size;
}Stack;

Stack * CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max = 20, i = 0, a, b, result;
	int error_flag = 0;
	fgets(input_str, 101, fin);
	stack = CreateStack(max);

	fprintf(fout, "Top Numbers :");
	while(input_str[i] != '#' && error_flag == 0){		// Check the error_flag when it is needed.
		switch(input_str[i]){
			case '+':
				if(IsEmpty(stack)){
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				b = Pop(stack);
				if(IsEmpty(stack) && error_flag == 0){		// Check the error_flag with the 2nd pop operation.
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				a = Pop(stack);
				result = a+b;
				Push(stack, result);
				break;
			case '-':
				if(IsEmpty(stack)){
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				b = Pop(stack);
				if(IsEmpty(stack) && error_flag == 0){		// Check the error_flag with the 2nd pop operation.
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				a = Pop(stack);
				result = a-b;
				Push(stack, result);
				break;
			case '*':
				if(IsEmpty(stack)){
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				b = Pop(stack);
				if(IsEmpty(stack) && error_flag == 0){		// Check the error_flag with the 2nd pop operation.
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				a = Pop(stack);
				result = a*b;
				Push(stack, result);
				break;
			case '/':
				if(IsEmpty(stack)){
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				b = Pop(stack);
				if(b == 0){		// Divide by zero error
					fprintf(fout, "\nError : Invalid Postfix Expression, Divide by Zero!\n");
					error_flag = 3;
					break;
				}
				if(IsEmpty(stack) && error_flag == 0){		// Check the error_flag with the 2nd pop operation.
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				a = Pop(stack);
				result = a/b;
				Push(stack, result);
				break;
			case '%':
				if(IsEmpty(stack)){
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
                b = Pop(stack);
				if(b == 0){
                    fprintf(fout, "\nError : Invalid Postfix Expression, Divide by Zero!\n");
                    error_flag = 3;
                    break;
                }
                if(IsEmpty(stack) && error_flag == 0){		// Check the error_flag with the 2nd pop operation.
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Empty!\n");
					error_flag = 2;
					break;
				}
				a = Pop(stack);
				result = a%b;
				Push(stack, result);
				break;
			default:
				// Push the number if the stack is not full.
				if(IsFull(stack)){
					fprintf(fout, "\nError : Invalid Postfix Expression, Stack is Full!\n");
					error_flag = 1;
					break;
				}else{
					Push(stack, ((int)input_str[i])-48);
					break;
				}
		}
		if(error_flag == 0)
			fprintf(fout, " %d", Top(stack));
		i++;
	}
	if(error_flag == 0){
		if(stack->top != 0)     // If stack is not empty
			fprintf(fout, "\nError : Invalid Postfix Expression, [%d] numbers are left!\n", stack->top+1);
		else                    // If stack is empty
			fprintf(fout, "\nEvaluation Result : %d\n", Top(stack));
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack* CreateStack(int max){    // Create a stack with dynamic allocation.
	Stack* S = NULL;
	S = (Stack*)malloc(sizeof(Stack));
	S->key = (int*)malloc(sizeof(int)*max);
	S->max_stack_size = max;
	S->top = -1;        // Stack's top index is -1 when the stack is empty.

	return S;
}

void Push(Stack* S, int X){     // Push a new element, print an error message when it is full.
    S->key[++S->top] = X;

    return;
}

int Pop(Stack* S){      // Pop the top element. If stack is empty, print an error message.
    int rtn = S->key[S->top--];     // Point the top and decrease by 1

    return rtn;
}

int Top(Stack* S){      // Return the top element.
    return S->key[S->top];
}

void DeleteStack(Stack* S){     // Free all memory of the stack and delete stack.
    free(S->key);
	free(S);
    
    return;
}

int IsEmpty(Stack* S){      // Return 1 if the stack is empty. Return 0 for else.
    return S->top == -1;
}

int IsFull(Stack* S){       // Return 1 if the stack is full. Return 0 for else.
    return S->top == S->max_stack_size-1;
}
