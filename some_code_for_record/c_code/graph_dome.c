/*************************************************************************
	> File Name: graph_dome.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 01 Oct 2025 11:54:46 PM CST
 ************************************************************************/

#include <stdio.h>

typedef struct{
	int capacity;
	int size;
	void** data;
} scf_vector_t;

typedef struct{
	scf_vector_t* prevs; // 动态数组 
	scf_vector_t* nexts; // 动态数组
	int			  flag;	 // 有没有访问过
	int				d;	 // 携带的数据
} graph_t;



int main(void){

	return 0;
}

