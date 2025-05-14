/*************************************************************************
	> File Name: code01.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 14 May 2025 03:55:17 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;

template <typename T>
T maximum(T a, T b){
	return (a > b)? a : b;
}

int main(void){
	int int_max = maximum(5,10);
	cout << "Max of 5 and 10 : " << int_max << endl;

	double double_max = maximum(3.14,2.71);
	cout << "Max of 3.14 and 2.71 : "<< double_max << endl;

	string string_max = maximum(string("apple"),string("banana"));
	cout << "Max of 'apple' and 'banana' : " << string_max << endl;

	return 0;
}
