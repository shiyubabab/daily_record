/*************************************************************************
	> File Name: sort.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 10 Apr 2025 09:19:46 AM CST
 ************************************************************************/

#include<iostream>
using namespace std;

const int N = 1e6 + 10;
int n;
int q[N];

void quick_sort(int *q,int l,int r){
	if(l>=r) return;

	int x = q[l], i = l-1, j = r+1;
	while(i<j){
		do i++;while(q[i]<x);
		do j--;while(q[j]>x);
		if(i<j){
			int tmp = q[i];
			q[i] = q[j];
			q[j] = tmp;
		}
	}
	quick_sort(q,l,j);
	quick_sort(q,j+1,r);
}

int main(){
	scanf("%d",&n);
	for(int i = 0;i<n;i++) scanf("%d",&q[i]);

	quick_sort(q,0,n-1);

	for(int i = 0;i<n;i++) printf("%d ",q[i]);
	return 0;
}
