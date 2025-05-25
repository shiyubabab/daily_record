/*************************************************************************
	> File Name: merge_sort.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 10 Apr 2025 09:43:58 AM CST
 ************************************************************************/

#include<iostream>
using namespace std;

const int N = 1e6 + 10;
int n;
int q[N],tmp[N];

void merge_sort(int *q,int left,int right){
	if(left >= right) return;
	int mid = left + right >> 1;
	merge_sort(q,left,mid); merge_sort(q,mid+1,right);
	int k = 0, i = left, j = mid+1;
	while(i<=mid && j<=right){
		if(q[i]<=q[j]) tmp[k++] = q[i++];
		else tmp[k++] = q[j++];
	}
	while(i<=mid) tmp[k++] = q[i++];
	while(j<=right) tmp[k++] = q[j++];
	for(int qi=left,ti=0 ; qi<=right ; qi++,ti++) q[qi] = tmp[ti];
}

int main(void){
	scanf("%d",&n);
	for(int i = 0 ; i < n; i++) scanf("%d",&q[i]);
	for(int i = 0 ; i < n; i++) printf("%d ",q[i]);
	cout << endl;
	merge_sort(q,0,n-1);
	for(int i = 0 ; i < n; i++) printf("%d ",q[i]);

	return 0;
}
