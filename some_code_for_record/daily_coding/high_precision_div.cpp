/*************************************************************************
	> File Name: high_precision_mul.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 27 May 2025 03:53:35 PM CST
 ************************************************************************/

#include<iostream>
#include<vector>
using namespace std;

vector<int> div(vector<int> &A,int &B){
	vector<int> C;
	int t = 0;
	for(int i = 0;i<A.size() || t;i++){
		if(i < A.size()) t += A[i] * B;
		C.push_back(t%10);
		t = t / 10;
	}
	while(C.size()>1 && C.back() == 0) C.pop_back();
	return C;
}


int main(void){
	string a;
	int B;
	cin >> a >> B;
	vector<int> A,C;
	for(int i = a.size() - 1;i>=0;i--) A.push_back(a[i] - '0');
	C = div(A,B);
	for(int i = C.size() - 1;i>=0;i--) cout<<C[i];
	cout << endl;
	return 0;
}
