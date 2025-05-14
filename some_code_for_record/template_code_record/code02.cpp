/*************************************************************************
	> File Name: code02.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 14 May 2025 04:02:17 PM CST
 ************************************************************************/

#include<iostream>
#include<string>
using namespace std;

template <typename T1,typename T2>
class Pair{
	public:
		Pair(T1 first,T2 second): first_(first),second_(second){}
		T1 getFirst() const { return first_;}
		T2 getSecond() const { return second_;}

		void setFirst(T1 first) { first_ = first;}
		void setSecond(T2 second) { second_ = second;}
	private:
		T1 first_;
		T2 second_;
};

int main(void){
	Pair<int,double> p1(10,3.14);
	cout << "Pair 1: (" << p1.getFirst() << ", "<< p1.getSecond()<< ")" << endl;

	Pair<string,int> p2(string("hello"),123);
	cout << "Pair 2: (" << p2.getFirst() << ", "<< p2.getSecond()<< ")" << endl;

	p2.setFirst("world");
	cout << "Pair 2 (modified): (" << p2.getFirst() << ", "<< p2.getSecond()<< ")" << endl;

	return 0;
}
