/*************************************************************************
	> File Name: orthogonal.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 16 Apr 2025 05:09:16 PM CST
 ************************************************************************/

#include<iostream>
#include<Eigen/Dense>
using namespace std;

Eigen::MatrixXf least_squared(Eigen::MatrixXf _A,Eigen::MatrixXf _b){
	Eigen::MatrixXf ret;
	Eigen::MatrixXf A=_A,b=_b;
	Eigen::MatrixXf P = A * (A.transpose()*A).inverse() * A.transpose();
	Eigen::MatrixXf p = P*b;
	ret = A.colPivHouseholderQr().solve(p);
	return ret;
}


Eigen::MatrixXf orthogonal(Eigen::MatrixXf _A){
	Eigen::MatrixXf A = _A;
	//cout<< "Matrix A:\n" << A << endl;

	Eigen:: HouseholderQR<Eigen::MatrixXf> qr(A);

	Eigen:: MatrixXf Q = qr.householderQ();
	//cout << "\nQ:\n"<< Q <<endl;

	//Eigen:: MatrixXf R = qr.matrixQR().triangularView<Eigen::Upper>();
	//cout << "\nR:\n"<< R <<endl;

	//Eigen:: MatrixXf A_restore = Q*R;
	//cout << "\nQ*R:\n"<<A_restore<<endl;

	//Eigen:: MatrixXf QtQ = Q.transpose() * Q;
	//cout<< "\nQ^T * Q:\n"<<QtQ<<endl;

	return Q;
}

Eigen::MatrixXf least_squared_with_orthogonal(Eigen::MatrixXf _A,Eigen::MatrixXf _b){
	Eigen::MatrixXf ret;
	Eigen::MatrixXf A = _A,b = _b;
	Eigen::MatrixXf Q = orthogonal(A);
	Eigen::MatrixXf p = Q*Q.transpose()*b;
	ret = A.colPivHouseholderQr().solve(p);
	return ret;
}


int main(void){
	int m,n;
	int number;
	cout << "Please input the number of row and column." << endl;
	scanf("%d %d",&m,&n);
	number = m*n;
	Eigen::MatrixXf A(m,n);
	cout << "\nPlease input matrix or the column space of A" << endl;
	for(int i = 0;i<m;i++){
		for(int j = 0;j<n;j++){
			cin >> A(i,j);
		}
	}
	cout << "\nA:\n" << A << endl;
	//A << 1,1,
		 //1,2,
		 //1,3;

	Eigen::MatrixXf b(m,1);
	cout << "\nPlease input matrix or vector of b" << endl;
	for(int i = 0;i<m;i++){
		cin>>b(i);
	}
	cout << "\nb:\n" << b << endl;
	//b<<1,2,2;


	Eigen::MatrixXf ret_without_orthogonal = least_squared(A,b);
	cout<< "\nret without orthogonal:\n"<< ret_without_orthogonal <<endl;


	Eigen::MatrixXf ret_with_orthogonal = least_squared_with_orthogonal(A,b);
	cout<< "\nret with orthogonal:\n"<< ret_with_orthogonal <<endl;

	Eigen::MatrixXf Q = orthogonal(A);
	cout<< "\nthe result of orthogonal, Q:\n"<< Q <<endl;

	return 0;
}
