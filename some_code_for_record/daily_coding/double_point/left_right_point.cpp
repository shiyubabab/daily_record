/*************************************************************************
	> File Name: left_right_point.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 05 Jun 2025 10:32:48 AM CST
 ************************************************************************/

#include<iostream>
#include<vector>
using namespace std;

const int N = 1e5+10;
int n;
int t[N];

int find_max_area(vector<int> &q){
	if(q.size()<=1) return 0;
	int res = 0,n=q.size();
	for(int i = 0;i<n;i++){
		for(int j = n-1;j>i;j--){
			int L = j - i;
			int H = min(q[i],q[j]);
			res = max(res,L*H);
		}
	}
	return res;
}


int find_max_area_nice(vector<int> &q){
	if(q.size()<=1) return 0;
	int res = 0,l=0,r=q.size();
	while(l<r){
		int L = r-l;
		int H = min(q[r],q[l]);
		res = max(res,L*H);
		if(q[l] <= q[r] && l<r) l++;
		else r--;
	}
	return res;
}


int main(void){
	cin >> n;
	vector<int> q;
	for(int i = 0;i<n;i++) scanf("%d",&t[i]);
	for(int i = 0;i<n;i++) q.push_back(t[i]);
	int res = find_max_area_nice(q);
	cout << res << endl;
	return 0;
}
