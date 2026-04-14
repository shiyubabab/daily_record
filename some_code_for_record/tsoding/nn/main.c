/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 08 Apr 2026 01:35:31 PM CST
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#define NN_IMPLEMENTATION
#include "nn.h"

#define OP_LIST(XX)	\
	XX(a0, 1, 2)	\
	XX(w1, 2, 2)	\
	XX(b1, 1, 2)	\
	XX(a1, 1, 2)	\
	XX(w2, 2, 1)	\
	XX(b2, 1, 1)	\
	XX(a2, 1, 1)

#define XOR_GEN(name, rows, cols) Mat name;
typedef struct{
	OP_LIST(XOR_GEN)
} Xor;
#undef XOR_GEN

void forward_xor(Xor m)
{
	mat_dot(m.a1, m.a0, m.w1);
	mat_add(m.a1,m.b1);
	mat_act(m.a1,sigmoidf);

	mat_dot(m.a2, m.a1, m.w2);
	mat_add(m.a2,m.b2);
	mat_act(m.a2,sigmoidf);
}

float cost(Xor m, Mat ti, Mat to)
{
	assert(ti.rows == to.rows);
	assert(to.cols == m.a2.cols);
	size_t n = ti.rows;

	float result = 0.f;
	for(size_t i = 0;i<n;++i){
		Mat x = mat_row(ti,i);
		Mat y = mat_row(to,i);

		mat_copy(m.a0,x);
		forward_xor(m);

		size_t q = to.cols;
		for(size_t j = 0;j<q;++j){
			float d = MAT_AT(m.a2,0,j) - MAT_AT(y,0,j);
			result += d*d;
		}
	}
	return result/n;
}

Xor xor_alloc(void)
{
	Xor m;
#define MAT_GEN(name, rows, cols) m.name = mat_alloc(rows,cols);
	OP_LIST(MAT_GEN)
#undef MAT_GEN
	return m;
}

void finite_diff(Xor m,Xor g,float eps,Mat ti,Mat to)
{

#define DIFF(x) do{\
	for(size_t i = 0;i<m.x.rows;++i){						\
		for(size_t j = 0;j<m.x.cols;++j){					\
			float saved = MAT_AT(m.x,i,j);					\
			MAT_AT(m.x,i,j) += eps;							\
			float add_c = cost(m,ti,to);					\
			MAT_AT(m.x,i,j) = saved;						\
			MAT_AT(m.x,i,j) -= eps;							\
			float mul_c = cost(m,ti,to);					\
			MAT_AT(m.x,i,j) = saved;						\
			MAT_AT(g.x,i,j) = (add_c - mul_c)/(2.f*eps);	\
		}													\
	}														\
}while(0)

	DIFF(w1);
	DIFF(b1);
	DIFF(w2);
	DIFF(b2);
#undef DIFF
}

void learning(Xor m, Xor g, float rate)
{

#define LEARN(x) do{										\
	for(size_t i = 0;i<m.x.rows;++i){						\
		for(size_t j = 0;j<m.x.cols;++j){					\
			MAT_AT(m.x,i,j) -= rate*MAT_AT(g.x,i,j);		\
		}													\
	}														\
} while(0)

	LEARN(w1);
	LEARN(b1);
	LEARN(w2);
	LEARN(b2);
#undef LEARN

}

float td[] = {
	0, 0, 0,
	0, 1, 1,
	1, 0, 1,
	1, 1, 0,
};

int main(void)
{
	srand(time(0));

	size_t r = sizeof(td) / sizeof(*td) / 3;
	Mat ti = {
		.rows = r,
		.cols = 2,
		.stride = 3,
		.es = td,
	};

	Mat to = {
		.rows = r,
		.cols = 1,
		.stride = 3,
		.es = td + 2,
	};

	float rate = 1e-1;

#if 0
	Xor m = xor_alloc();
	Xor g = xor_alloc();

	mat_rand(m.w1,0,1);
	mat_rand(m.b1,0,1);
	mat_rand(m.w2,0,1);
	mat_rand(m.b2,0,1);

	for(int i = 0;i<100*1000;i++){
		finite_diff(m,g,eps,ti,to);
		learning(m,g,rate);
		printf("loss = %f\n",cost(m,ti,to));
	}

	for(size_t i = 0;i<2;++i){
		for(size_t j = 0;j<2;++j){
			MAT_AT(m.a0,0,0) = i;
			MAT_AT(m.a0,0,1) = j;
			forward_xor(m);
			//printf("%zu ^ %zu = %f\n",i , j , *m.a2.es);
		}
	}
#endif


	size_t arch[] = {2, 2, 1};
	NN nn = nn_alloc(arch,ARCH_LAYERS(arch));
	NN gn = nn_alloc(arch,ARCH_LAYERS(arch));
	nn_rand(nn,0,1);

	for(int i = 0;i<500*1000;i++){
		//float eps  = 1e-1;
		//nn_finite_diff(nn,gn,eps,ti,to);
		nn_backprop(nn,gn,ti,to);
		nn_learn(nn,gn,rate);
		printf("loss = %f\n",nn_cost(nn,ti,to));
	}

	for(size_t i = 0;i<2;++i){
		for(size_t j = 0;j<2;++j){
			MAT_AT(NN_INPUT(nn),0,0) = i;
			MAT_AT(NN_INPUT(nn),0,1) = j;
			nn_forward(nn);
			printf("%zu ^ %zu = %f\n",i , j , MAT_AT(NN_OUTPUT(nn),0,0));
		}
	}

	NN_DUMP(nn);

	return 0;
}

