/*************************************************************************
	> File Name: kmp.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 03 Jun 2025 03:37:38 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;
const int N = 10010;
int n;
int q[N];

int main(void){
	cin >> n;
	for(int i = 0;i<n;i++) scanf("%d",&q[i]);

	int res = 1;
	for(int i = 0 , j = 0;i<n;i++){
		while(j<=i && q[j] == q[i]) j++;
		res = max(res,i - j +1);
	}
	cout << res << endl;
	return 0;
}
