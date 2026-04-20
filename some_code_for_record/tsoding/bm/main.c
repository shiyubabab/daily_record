/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 20 Apr 2026 09:22:17 AM CST
 ************************************************************************/

#ifndef HELLOWORLD
#include <stdio.h>
#define HELLOWORLD printf
#endif

#ifndef BM_ASSERT
#include <assert.h>
#define BM_ASSERT assert
#endif

#include <stdlib.h>
#include <string.h>

#define BM_STACK_CAPACITY 1024
#define BM_PROGRAM_CAPACITY 1024
#define BM_EXECUTION_LIMIT 69
typedef int64_t Word;

typedef enum {
	TRAP_OK = 0,
	TRAP_STACK_OVERFLOW,
	TRAP_STACK_UNDERFLOW,
	TRAP_ILLEGAL_INST,
	TRAP_ILLEGAL_INST_ACCESS,
	TRAP_DIV_BY_ZERO,
} Trap;

const char *trap_as_cstr(const Trap trap)
{
	switch(trap){
		case TRAP_OK: return "TRAP_OK";
		case TRAP_STACK_OVERFLOW: return "TRAP_STACK_OVERFLOW";
		case TRAP_STACK_UNDERFLOW: return "TRAP_STACK_UNDERFLOW";
		case TRAP_ILLEGAL_INST: return "TRAP_ILLEGAL_INST";
		case TRAP_ILLEGAL_INST_ACCESS: return "TRAP_ILLEGAL_INST_ACCESS";
		case TRAP_DIV_BY_ZERO: return "TRAP_DIV_BY_ZERO";
		default: BM_ASSERT(0 && "trap_as_cstr : Unreachable");
	}
}

typedef enum {
	INST_PUSH,
	INST_PLUS,
	INST_MINUS,
	INST_MULT,
	INST_DIV,
	INST_JMP,
	INST_HALT, //interrupt or stop ?
} Inst_Type;

const char *inst_type_as_cstr(Inst_Type type)
{
	switch(type) {
		case INST_PUSH: return "INST_PUSH";
		case INST_PLUS: return "INST_PLUS";
		case INST_MINUS:return "INST_MINUS";
		case INST_MULT: return "INST_MULT";
		case INST_DIV:  return "INST_DIV";
		case INST_JMP: return "INST_JMP";
		case INST_HALT:  return "INST_HALT";
		default: BM_ASSERT(0 && "inst_type_as_cstr : Unreachable");
	}

}

typedef struct {
	Inst_Type type;
	Word operand;
} Inst;

typedef struct {
	Word stack[BM_STACK_CAPACITY];
	Word stack_size;

	Inst program[BM_PROGRAM_CAPACITY];
	Word program_size;
	Word ip;

	Word halt;
} Bm;

#define MAKE_INST_PUSH(value) (Inst){.type = INST_PUSH, .operand = (value)}
#define MAKE_INST_PLUS		  (Inst){.type = INST_PLUS}
#define MAKE_INST_MINUS		  (Inst){.type = INST_MINUS}
#define MAKE_INST_MULT		  (Inst){.type = INST_MULT}
#define MAKE_INST_DIV		  (Inst){.type = INST_DIV}
#define MAKE_INST_JMP(addr)	  (Inst){.type = INST_JMP, .operand = (addr)}
#define MAKE_INST_HALT		  (Inst){.type = INST_HALT}

Trap bm_execute_inst(Bm *bm)
{
	if(bm->ip < 0 || bm->ip >= bm->program_size){
		return TRAP_ILLEGAL_INST_ACCESS;
	}
	Inst inst = bm->program[bm->ip];

	switch(inst.type){
		case INST_PUSH:
			if(bm->stack_size >= BM_STACK_CAPACITY){
				return TRAP_STACK_OVERFLOW;
			}
			bm->stack[bm->stack_size++] = inst.operand;
			bm->ip += 1;
			break;
		case INST_PLUS:
			if(bm->stack_size < 2){
				return TRAP_STACK_UNDERFLOW;
			}
			bm->stack[bm->stack_size - 2] += bm->stack[bm->stack_size - 1];
			bm->stack_size -= 1;
			bm->ip += 1;
			break;
		case INST_MINUS:
			if(bm->stack_size < 2){
				return TRAP_STACK_UNDERFLOW;
			}
			bm->stack[bm->stack_size - 2] -= bm->stack[bm->stack_size - 1];
			bm->stack_size -= 1;
			bm->ip += 1;
			break;
		case INST_MULT:
			if(bm->stack_size < 2){
				return TRAP_STACK_UNDERFLOW;
			}
			bm->stack[bm->stack_size - 2] *= bm->stack[bm->stack_size - 1];
			bm->stack_size -= 1;
			bm->ip += 1;
			break;
		case INST_DIV:
			if(bm->stack_size < 2) return TRAP_STACK_UNDERFLOW;
			if(bm->stack[bm->stack_size-1] == 0) return TRAP_DIV_BY_ZERO;

			bm->stack[bm->stack_size - 2] /= bm->stack[bm->stack_size - 1];
			bm->stack_size -= 1;
			bm->ip += 1;
			break;
		case INST_JMP:
			bm->ip = inst.operand; //????
			break;
		case INST_HALT:
			bm->halt = 1;
			break;
		default:
			return TRAP_ILLEGAL_INST;
	}
	return TRAP_OK;
}

void bm_load_program_from_memory(Bm *bm, Inst *program, size_t program_size)
{
	BM_ASSERT(program_size < BM_PROGRAM_CAPACITY);
	memcpy(bm->program,program,program_size*sizeof(program[0]));
	bm->program_size = program_size;
}

void bm_dump(Bm bm, char *name, size_t padding)
{
	HELLOWORLD("%*s%s [size:%zu] stack = [\n",(int)padding,"",name,bm.stack_size);
	
	if(bm.stack_size == 0) HELLOWORLD("%*s%s\n",(int)padding+4,"","[empty]");
	for(Word i = 0;i < bm.stack_size;++i){
		HELLOWORLD("%*s %ld\n",(int)padding+4,"",bm.stack[i]);
	}
	HELLOWORLD("%*s]\n",(int)padding,"");

}
#define BM_DUMP(bm) bm_dump(bm,#bm,0)

#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))
Bm bm = {0};
Inst program[] = {
	MAKE_INST_PUSH(0),		//0
	MAKE_INST_PUSH(1),		//1
	MAKE_INST_PLUS,			//2
	MAKE_INST_JMP(1),
};


void bm_push_inst(Bm *bm, Inst inst)
{
	BM_ASSERT(bm->program_size < BM_PROGRAM_CAPACITY);
	bm->program[bm->program_size++] = inst;
}

int main(void)
{
	bm_load_program_from_memory(&bm,program,ARRAY_SIZE(program));
	BM_DUMP(bm);
	for(int i = 0; i < BM_EXECUTION_LIMIT && !bm.halt; i++){
		HELLOWORLD("%s\n",inst_type_as_cstr(program[bm.ip].type));
		Trap trap = bm_execute_inst(&bm);
		BM_DUMP(bm);
		if(trap != TRAP_OK){
			fprintf(stderr,"ERROR: Trap activated [%s]\n",trap_as_cstr(trap));
			exit(1);
		}
	}

	return 0;
}

