/*************************************************************************
	> File Name: 184_HZOJ.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 09 Apr 2025 02:13:00 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int number_pach(int n){
	if(n==1){
		return 1;
	}
	return (number_pach(n-1)+1)*2;
}

int main(void){
	int num = number_pach(3);
	printf("number of pach is [%d]\n",num);
	return 0;
}

