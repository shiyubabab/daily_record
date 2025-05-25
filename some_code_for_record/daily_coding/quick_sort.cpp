/*************************************************************************
	> File Name: sort.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 10 Apr 2025 09:19:46 AM CST
 ************************************************************************/

#include<iostream>
using namespace std;

const int N = 1e6+10;
int n;
int q[N];

void swap(int *a,int *b){
}

void quick_sort(int *q,int left,int right){
	if(left>=right) return;
	int x = q[left],l = left - 1,r = right + 1;
	while(l<r){
		do {l++;} while(l<r && q[l] < x);
		do {r--;} while(l<r && q[r] > x);
		if(l<r){
			int temp = q[l];
			q[l] = q[r];
			q[r] = temp;
		}
	}
	quick_sort(q,left,r);
	quick_sort(q,r+1,right);
}

int main(void){
	scanf("%d",&n);
	for(int i = 0 ; i < n ; i++) scanf("%d",&q[i]);
	for(int i = 0 ; i < n ; i++) printf("%d ",q[i]);
	printf("\n");
	quick_sort(q,0,n-1);
	for(int i = 0 ; i < n ; i++) printf("%d ",q[i]);

}
