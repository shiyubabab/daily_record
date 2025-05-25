/*************************************************************************
	> File Name: Cube_Root_of_a_Number.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sat 24 May 2025 07:30:41 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;

int main(void){
	double x;
	cin >> x;
	double l = -10000,r = 10000;
	while( r-l > 1e-8){
		double mid = (l+r) / 2;
		if(mid*mid*mid <= x) l = mid;
		else r = mid;
	}
	cout << l <<endl;
}
