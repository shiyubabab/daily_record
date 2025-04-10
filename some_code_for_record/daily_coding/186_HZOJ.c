/*************************************************************************
	> File Name: 186_HZOJ.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 09 Apr 2025 02:27:05 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#define N 5

int step[N] = {2, 2, 3, 1, 2};

int num_of_step(int i){	//小球从第i个位置开始被弹出弹簧板的次数
	if(i>=N){
		return 0;
	}
	return num_of_step(i+step[i])+1;

}

int main(void){
	printf("number of step is [%d]\n",num_of_step(0));
	return 0;
}
