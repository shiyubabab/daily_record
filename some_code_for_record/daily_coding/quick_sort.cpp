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


void quick_sort(int *q,int left,int right){
	if(left>=right) return;
	int k=q[left],l = left -1,r = right + 1;
	while(l<r){
		do{l++;} while( l<r && k>q[l]);
		do{r--;} while( l<r && k<q[r]);
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
	printf("\n");
	quick_sort(q,0,n-1);
	for(int i = 0 ; i < n ; i++) printf("%d ",q[i]);

}
