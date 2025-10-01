/*************************************************************************
	> File Name: hong.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 22 Sep 2025 08:57:08 PM CST
 ************************************************************************/

#include<stdio.h>
#define A(x) #x

struct mlxh{
	int id;
	char *name;
};

int main(void){
	struct mlxh a = {
		.id = 0,
		.name = A(mlxh),
	};
	printf("name is %s\n",a.name);
	return 0;
}

