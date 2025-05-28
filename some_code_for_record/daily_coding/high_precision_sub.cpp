/*************************************************************************
	> File Name: high_precision_sub.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 27 May 2025 01:43:16 PM CST
 ************************************************************************/

#include<iostream>
#include<vector>
using namespace std;

vector<int> sub(vector<int> &A,vector<int> &B){
	vector<int> C;
	int t = 0;
	for(int i = 0;i<A.size();i++){
		t = A[i] - t;
		if(i<B.size()) t -= B[i];
		C.push_back((t+10)%10);
		if(t<0) t = 1;
		else t = 0;
	}
	while(C.size()>1 && C.back() == 0) C.pop_back();
	return C;
}

bool cmp(vector<int> &A,vector<int> &B){
	if(A.size() != B.size()) return A.size() > B.size();
	for(int i = A.size() - 1;i>=0;i--){
		if(A[i] != B[i]) return A[i] > B[i];
	}
	return true;
}

int main(void){
	string a,b;
	cin >> a >> b;
	vector<int> A,B,C;
	for(int i = a.size() -1;i>=0;i--) A.push_back(a[i] - '0');
	for(int i = b.size() -1;i>=0;i--) B.push_back(b[i] - '0');
	if(cmp(A,B)){
		C = sub(A,B);
		for(int i = C.size() -1;i>=0;i--) cout<<C[i];
	}else{
		C = sub(B,A);
		cout << "-";
		for(int i = C.size() -1;i>=0;i--) cout<<C[i];
	}
	cout << endl;
	return 0;
}
