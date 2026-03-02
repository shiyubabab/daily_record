/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 01 Mar 2026 03:18:46 PM CST
 ************************************************************************/


#define MAKE_SOME_NOISE
#include "frame.h"

int main(int argc,char *argv[]){
	int n = 3;
	hanoi_nr(n,'A','B','C');
	return 0;
}

