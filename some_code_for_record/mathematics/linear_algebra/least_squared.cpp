/*************************************************************************
	> File Name: eigenDemo.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 13 Apr 2025 02:01:30 PM CST
 ************************************************************************/

#include<iostream>
#include<Eigen/Dense>
using namespace std;


int main(void){
	Eigen::Matrix<float,3,2> A_32;
	A_32 << 1,1,
			1,2,
			1,3;
	cout << A_32 << endl;
	Eigen::Matrix<float,3,1> b_31;
	b_31 << 1,2,2;
	cout << b_31 << endl;


	Eigen::Matrix<float,2,3> A_T_23 = A_32.transpose();
	cout << A_T_23 << endl;

	Eigen::Matrix<float,2,2> A_T_A_inverse = (A_T_23*A_32).inverse();
	cout << A_T_A_inverse << endl;

	Eigen::Matrix<float,3,3> P_33 = A_32* A_T_A_inverse * A_T_23;
	Eigen::Matrix<float,3,1> p_31=P_33 * b_31;
	cout << p_31 << endl;

	Eigen::Matrix<float,2,1> ret = A_32.colPivHouseholderQr().solve(p_31);
	cout << ret << endl;

	return 0;
}
