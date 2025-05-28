/*************************************************************************
	> File Name: submatrix_sun.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 28 May 2025 04:04:37 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;

#include<iostream>
const int N = 1010;
int n,m,q;
int a[N][N],s[N][N];

int main(void){
	scanf("%d%d%d",&n,&m,&q);
	for(int y = 0;y<n;y++)
		for(int x = 0;x<m;x++)
			scanf("%d",&a[y][x]);
			            
	for(int y = 0;y<n;y++)
		for(int x = 0;x<m;x++)
			s[y][x] = s[y-1][x] + s[y][x-1] - s[y-1][x-1] + a[y][x];
				            
	while(q--){
		int x1,y1,x2,y2;
		scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
		printf("%d\n",s[x2][y2] - s[x1-1][y2] - s[x2][y1-1] + s[x1-1][y1-1]);
	}
	return 0;
}
					
