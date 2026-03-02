/*************************************************************************
	> File Name: frame.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 01 Mar 2026 02:51:31 PM CST
 ************************************************************************/

#ifndef __FRAME_H__
#define __FRAME_H__

#include <stdio.h>
#include <assert.h>

struct Frame {
	//Each frame has a program counter to keep track its next execute.
	//to-be-executed statement.
	int pc;

	int n;
	char from,to,via;
};

typedef struct Frame Frame; 

void hanoi_nr(int n, char from, char via, char to);
extern void hanoi_nr(int n, char from, char via, char to);

void hanoi(int n, char from, char via, char to);
extern void hanoi(int n, char from, char via, char to);

#endif

#ifdef MAKE_SOME_NOISE

//#define ret(val) ({top--;retval = (val);})

void hanoi_nr(int n, char from, char via, char to)
{
	Frame stk[64];
	Frame *top = stk - 1;
/*
----------------	stk
top	|n	| <--  sp
	|from|
	|pc 1|
	------
	|	| <-- 
	|	|
	|pc |
	------
	|	| <-- 
	|	|
	|	|
----------------
*/

#define call(...) ({*(++top) = (Frame){.pc = 0, __VA_ARGS__};})
#define ret() ({top--;})

	call(n,from,to,via);
	while(1){
		Frame *f = top;
		if(top < stk){
			break;
		}

		int next_pc = f->pc + 1;
		int n = f->n,from = f->from, to = f->to,via = f->via;

		switch(f->pc){
			case 0:
				if(f->n <= 0){
					ret();
					break;
				}
				call(f->n - 1, f->from, f->to, f->via);
				break;
			case 1:
				printf("Move %d:%c->%c\n",f->n,f->from,f->to);
				call(f->n-1,f->via,f->from,f->to);
				break;
			case 2:
				ret();
				break;
			default:
				assert(0 && "Invalid PC state.");
		}
		f->pc = next_pc;
	}
}


void hanoi(int n, char from, char via, char to)
{
	if(n == 1){
		printf("pan 1：from %c to %c\n",from,to);
		return;
	}

	hanoi(n-1,from,to,via);

	printf("pan %d：from %c to %c\n",n,from,to);

	hanoi(n-1,via,from,to);

}

#endif
