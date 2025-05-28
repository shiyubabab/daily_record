/*************************************************************************
	> File Name: prefix_sum.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 28 May 2025 02:14:30 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;

const int N = 100010;
int n,m;
int a[N],s[N];

int main(void){
	scanf("%d%d",&n,&m);
	for(int i = 1;i<=n;i++) scanf("%d",&a[i]);
	for(int i = 1;i<=n;i++) s[i] = s[i-1] + a[i];
	while(m--){
		int l,r;
		scanf("%d%d",&l,&r);
		printf("%d\n",s[r]-s[l-1]);
	}
	return 0;
}
