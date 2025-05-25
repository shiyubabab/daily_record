/*************************************************************************
	> File Name: binary_search.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 10 Apr 2025 02:04:33 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;

const int N = 1e6+10;
int n,m;
int q[N];

int main(void){
	scanf("%d%d",&n,&m);
	for(int i = 0;i<n;i++) scanf("%d", &q[i]);
	while(m--){
		int x;
		scanf("%d",&x);
		int l = 0,r = n-1;
		while(l<r){
			int mid = l+r >> 1;
			if(x<=q[mid]) r = mid;
			else l = mid + 1;
		}
		if(q[l]!=x) cout << "-1 -1" << endl;
		else{
			cout << l << " ";
			int l = 0,r=n-1;
			while(l<r){
				int mid = l+r+1 >> 1;
				if(x>=q[mid]) l = mid;
				else r = mid - 1;
			}
			cout << l << endl;
		}
	}
	return 0;
}
