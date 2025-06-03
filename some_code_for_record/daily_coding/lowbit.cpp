/*************************************************************************
	> File Name: lowbit.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 03 Jun 2025 02:59:08 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;

int lowbit(int x){
	return x & -x;
}

int main(void){
	int n;
	cin >> n;
	while(n--){
		int x;
		cin >> x;
		int res = 0;
		while(x) x -= lowbit(x) , res++;
		cout << res << endl;
	}
	return 0;
}
