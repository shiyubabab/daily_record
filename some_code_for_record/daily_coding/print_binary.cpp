/*************************************************************************
	> File Name: print_binary.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 03 Jun 2025 02:51:35 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;


int main(void){
	int n = 10;
	for(int k = 3;k>=0;k--) cout << (n>>k & 1) <<endl;
	return 0;
}
