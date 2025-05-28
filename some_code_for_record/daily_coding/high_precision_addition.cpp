/*************************************************************************
	> File Name: high_precision_addition.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 26 May 2025 11:56:56 AM CST
 ************************************************************************/

#include<iostream>
#include<vector>
using namespace std;

vector<int> add(vector<int> &A,vector<int> &B){
	if(A.size() < B.size()) return add(B,A);

	vector<int> C;
	int tmp = 0;
	for(int i = 0;i<A.size();i++){
		tmp += A[i];
		if(i<B.size()) tmp+=B[i];
		C.push_back(tmp%10);
		tmp = tmp / 10;
	}
	if(tmp) C.push_back(tmp);
	return C;
}


int main(void){
	string a,b;
	cin >> a >> b;
	vector<int> A,B,C;
	for(int i = a.size() -1;i>=0;i--) A.push_back(a[i] - '0');
	for(int i = b.size() -1;i>=0;i--) B.push_back(b[i] - '0');
	C = add(A,B);
	for(int i = C.size() -1;i>=0;i--) cout << C[i];
	cout << endl;
	return 0;
}
