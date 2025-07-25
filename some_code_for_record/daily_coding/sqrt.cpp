/*************************************************************************
	> File Name: sqrt.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 10 Apr 2025 08:28:17 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;

int main(void){
	double x;
	cin >> x;
	double l = 0,r = x;
	while(r-l > 1e-6){
		double mid = (l + r) /2;
		if(mid*mid <= x) l = mid;
		else r = mid;
	}
	cout << l <<endl;
}
