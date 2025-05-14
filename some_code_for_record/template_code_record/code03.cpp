/*************************************************************************
	> File Name: code03.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 14 May 2025 04:28:34 PM CST
 ************************************************************************/

#include <iostream>
#include <cstring>
using namespace std;

template <typename T>
void swap(T& a,T& b){
	T temp = a;
	a = b;
	b = temp;
}

template <>
void swap<char*>(char*& a,char*& b){
	char *temp = new char[strlen(a)+1];
	strcpy(temp,a);
	delete[] a;
	a = new char[strlen(b) + 1];
	strcpy(a,b);
	delete[] b;
	b = new char[strlen(temp) + 1];
	strcpy(b,temp);
	delete[] temp;
}

int main(){
	int x=5,y=10;
	swap(x,y);
	cout<< "int x: " << x << ", y: "<< y << endl;

	char* str1 = new char[6];
	strcpy(str1,"hello");
	char* str2 = new char[6];
	strcpy(str2,"world");
	swap<char*>(str1,str2);
	cout << "char* str1: " << str1 << ", str2: " << str2 << endl;
	delete[] str1;
	delete[] str2;
	return 0;
}

