/*************************************************************************
	> File Name: test.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 29 May 2025 03:28:37 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int main(void){
	char buffer[30];
	int value = 123;
	char *name = "world";
	int ret  = snprintf(buffer,sizeof(buffer),"hello %s,\"%d\"!",name,value);
	printf("Buffer : %s \n",buffer);
	return 0;
}


