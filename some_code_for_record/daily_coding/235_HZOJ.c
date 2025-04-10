/*************************************************************************
	> File Name: 235_HZOJ.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 09 Apr 2025 02:59:13 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int arr[10];

void print_result(int n){
	for(int i = 0;i<=n;i++){
		printf(" %d",arr[i]);
	}
	printf("\n");
}

void f(int i,int j,int n){
	if(j>n){
		return;
	}
	for(int k=j;k<=n;k++){
		arr[i] = k;
		print_result(i);
		f(i+1,k+1,n);
	}
	return;
}

int main(void){
	f(0,1,4);
	return 0;
}

