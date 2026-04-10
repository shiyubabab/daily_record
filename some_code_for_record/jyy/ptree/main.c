/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 02 Mar 2026 11:27:09 AM CST
 ************************************************************************/

#define MAKE_SOME_NOISE
#include "process.h"

int main(void){
	Process_vector proc = {0};
	load_processes(&proc);
	parent_construct(proc);
	for(int i = 0;i<proc.count;i++) {
		if(proc.items[i]->pid == 1){
			print_tree(proc.items[i],0);
		}
	}
	return 0;
}
