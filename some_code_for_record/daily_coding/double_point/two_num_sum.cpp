/*************************************************************************
	> File Name: two_num_sum.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 05 Jun 2025 11:25:32 AM CST
 ************************************************************************/

#include<iostream>
#include<vector>
using namespace std;

const int N = 1e6+10;
int n,target;
int q[N];

vector<int> two_sum(vector<int> &nums,int target){
	vector<int> ret = {-1,-1};
	int n = nums.size();
	for(int i = 0;i<n;i++){
		for(int j = i+1;j<n;j++){
			if((target - nums[i]) == nums[j]) ret[0]=nums[i],ret[1] = nums[j];
		}
	}
	return ret;
}

vector<int> two_sum_nice(vector<int> &nums,int target){
	vector<int> ret = {-1,-1};
	int n = nums.size();
	for(int i = 0;i<n;i++){
		for(int j = i + 1;j<n;j++){
			if((target - nums[i]) == nums[j]) ret[0]=nums[i],ret[1] = nums[j];
			if(nums[i] + nums[j] > target) break;
		}
	}
	return ret;
}


vector<int> two_sum_nice_plus(vector<int> &nums,int target){
	vector<int> ret = {-1,-1};
	int n = nums.size();
	for(int i = 0,j=0;i<n;i++){
		while((nums[i] + nums[j])<target) j++;
		if((target - nums[i]) == nums[j]) ret[0]=nums[i],ret[1] = nums[j];
	}
	return ret;
}

int main(void){
	cin >> n >> target;
	for(int i = 0;i<n;i++) scanf("%d",&q[i]);
	vector<int> nums;
	for(int i = 0;i<n;i++) nums.push_back(q[i]);
	vector<int> res = two_sum_nice_plus(nums,target);
	cout << res[0] << " "  << res[1] << endl;
	return 0;
}
