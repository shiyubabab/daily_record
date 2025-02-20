/*************************************************************************
	> File Name: hanoi.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 18 Feb 2025 05:14:41 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

typedef int data_t;
typedef struct list{
	int size_of_list;
	data_t *vector;
}list_t;

list_t malloc_init_list(const data_t *input){
	list_t ret;
	int size_of_input = sizeof(input) / sizeof(input[0]);
	ret.size_of_list = size_of_input;
	ret.vector = (data_t *)malloc(size_of_input*sizeof(data_t));
	for(int i = 0;i<size_of_input;i++){
		ret.vector[i] = input[i];
	}
	return ret;
}

int recursion_function(){
	int ret = 0;

	return recursion_function((right+a[i]),left)
}

int main(void){
	data_t *input = {2,3,5,7};
	list_t list = malloc_init_list(input);
	return 0;
}

