/*************************************************************************
	> File Name: find_space.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 03 Jun 2025 10:55:38 AM CST
 ************************************************************************/

#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

int main(void){
	char str[1000];
	gets(str);
	int n = strlen(str);
	for(int i = 0;i<n;i++){
		int j = i;
		while(j<n && str[i] != ' ') j++;
		for(int k = 0;k<j;k++) cout<< str[k];
		cout << endl;
		i = j;
	}
	return 0;
}
