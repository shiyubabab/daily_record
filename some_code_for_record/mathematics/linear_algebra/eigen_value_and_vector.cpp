/*************************************************************************
	> File Name: eigen_value_and_vector.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 21 Apr 2025 09:38:10 AM CST
 ************************************************************************/

#include<iostream>
#include<Eigen/Dense>
#include<Eigen/Eigenvalues>
#include<Eigen/LU>
using namespace std;

void __eigen(Eigen::MatrixXd _A,Eigen::VectorXd &_eigen_values,Eigen::MatrixXd &_eigen_vectors){
	Eigen::MatrixXd A = _A;
	Eigen::VectorXcd eigen_values;
	Eigen::MatrixXcd eigen_vectors;

	Eigen::EigenSolver<Eigen::MatrixXd> es(A);
	eigen_values = es.eigenvalues();
	eigen_vectors = es.eigenvectors();

	int eigen_values_size = eigen_values.size();
	_eigen_values.resize(eigen_values_size);
	_eigen_vectors.resize(A.rows(), eigen_values_size);

	bool all_real;


	for(int i = 0;i<eigen_values_size;++i){
		if(abs(eigen_values(i).imag())>1e-9){
			all_real = false;
			break;
		}
		_eigen_values(i) = eigen_values(i).real();
		_eigen_vectors.col(i) = eigen_vectors.col(i).real();

		_eigen_vectors.col(i) /= _eigen_vectors.col(i).norm(); // 归一化

	}

	if (all_real) {
		std::cout << "\n实数特征值:\n" << _eigen_values << std::endl;
		std::cout << "\n实数特征向量 (作为列):\n" << _eigen_vectors << std::endl;
	} else {
		std::cout << "\n矩阵包含复数特征值或特征向量，已输出复数结果。" << std::endl;
	}
}

int main(void){
	int m;
	scanf("%d",&m);
	Eigen::MatrixXd A(m,m);
	for(int i = 0;i<m;i++){
		for(int j = 0;j<m;j++){
			cin >> A(i,j);
		}
	}

	//Eigen::EigenSolver<Eigen::MatrixXd> es(A);
	//Eigen::VectorXcd eigen_values = es.eigenvalues();
	//Eigen::MatrixXcd eigen_vectors = es.eigenvectors();

	Eigen::VectorXd eigen_values;
	Eigen::MatrixXd eigen_vectors;

	__eigen(A,eigen_values,eigen_vectors);


	cout<<"\n A's eigen values is: \n"<<eigen_values<<endl;
	cout<<"\n A's eigen vector is: \n"<<eigen_vectors<<endl;

/*
   Fibonacci
   1 1 2 3 5 8 13 ...... F_k

   F_k+2 = F_k+1 + F_k
   F_k+1 = F_k+1

   |1 1|	|F_k+1|
   |1 0|	|F_k  |

    A   *	V_k+1 =  V_k+2 
	A^k *   V_1   =  V_k+2
	A   = S^-1 * a   * S
	A^k	= S^-1 * a^k * S

	eigen: {

			1.618  : 
			-0.618 :
   }


 */

	return 0;
}

