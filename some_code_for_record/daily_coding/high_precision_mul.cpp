/*************************************************************************
	> File Name: high_precision_mul.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 27 May 2025 03:53:35 PM CST
 ************************************************************************/

#include<iostream>
#include<vector>
using namespace std;

/*
   12 x 1000
   t = 0
   2 x 1000 = 2000
   t = 2000
   C[0] = 0;
   t = t/10 ==> t = 200
   1 x 1000 = 1000
   t = 1200
   C[1] = 0
   t = t/10 ==> t = 120
 */
vector<int> mul(vector<int> &A,int &B){
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
	C = mul(A,B);
	for(int i = C.size() - 1;i>=0;i--) cout<<C[i];
	cout << endl;
	return 0;
}
