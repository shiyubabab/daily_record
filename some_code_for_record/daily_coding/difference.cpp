/*************************************************************************
	> File Name: difference.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 28 May 2025 04:10:07 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;

const int N = 10010;
int n,m;
int a[N],s[N];

int main(void){
	scanf("%d%d",&n,&m);
	for(int i = 1;i<=n;i++) scanf("%d",&s[i]);
	for(int i = 1;i<=n;i++) a[i] = s[i] - s[i-1];

	while(m--){
		int l,r,c;
		scanf("%d%d%d",&l,&r,&c);
		a[l] += c;
		a[r+1] -= c;
		for(int i = 1;i<=n;i++) s[i] = s[i-1] + a[i];
	}
	for(int i = 1;i<=n;i++) cout<< s[i];
	cout<<endl;
	return 0;
}
