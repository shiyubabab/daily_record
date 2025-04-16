/*************************************************************************
	> File Name: orthogonal.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 16 Apr 2025 05:09:16 PM CST
 ************************************************************************/

#include<iostream>
#include<Eigen/Dense>
using namespace std;

int main(void){
	int m = 3,n=2;
	Eigen::MatrixXf A(m,n);
	A << 1,1,
		 1,2,
		 1,3;
	cout<< "Matrix A:\n" << A << endl;

	Eigen:: HouseholderQR<Eigen::MatrixXf> qr(A);

	Eigen:: MatrixXf Q = qr.householderQ();
	cout << "\nQ:\n"<< Q <<endl;

	Eigen:: MatrixXf R = qr.matrixQR().triangularView<Eigen::Upper>();
	cout << "\nR:\n"<< R <<endl;

	Eigen:: MatrixXf A_restore = Q*R;
	cout << "\nQ*R:\n"<<A_restore<<endl;

	Eigen:: MatrixXf QtQ = Q.transpose() * Q;
	cout<< "\nQ^T * Q:\n"<<QtQ<<endl;
	return 0;
}
