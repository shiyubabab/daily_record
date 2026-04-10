/*************************************************************************
	> File Name: train.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 06 Apr 2026 08:05:06 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef float sample[3];

sample or_train[] = {
	{0, 0, 0},
	{1, 0, 1},
	{0, 1, 1},
	{1, 1, 0},
};

sample * train = or_train;
#define train_count 4

#define weight_count 9
typedef struct{
	float ww[weight_count];
}weight;

weight ws = {
	.ww = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,1.f}
};


float sigmoidf(float x)
{
	return 1.0f/(1.0f + expf(-x));
}

float rand_float(void)
{
	return (float) rand() / (float) RAND_MAX;
}

void set_rand_float(void)
{
	for(int i = 0; i < train_count; ++i){
		ws.ww[i] = rand_float();
	}
}

float forward(weight w, float x1, float x2)
{
	float a = sigmoidf(w.ww[0]*x1 + w.ww[1]*x2 + w.ww[2]);
	float b = sigmoidf(w.ww[3]*x1 + w.ww[4]*x2 + w.ww[5]);
	return sigmoidf(w.ww[6]*a + w.ww[7]*b + w.ww[8]);
}

float cost(weight w)
{
	float result = 0.f;
	for(int i = 0; i < train_count; ++i){
		float x1 = train[i][0];
		float x2 = train[i][1];

		float y= forward(w,x1,x2);
		float d = train[i][2] - y;
		result += d * d;
	}
	result /= train_count;
	return result;
}

weight partial_derivative(weight w, float eps)
{
	weight w_saved = w;
	weight ret;
	for(int i = 0; i<weight_count ; ++i){
		weight w_tmp   = w_saved;
		w_tmp.ww[i] = w_saved.ww[i] + eps;
		float add_c = cost(w_tmp);
		w_tmp.ww[i] = w_saved.ww[i] - eps;
		float mul_c = cost(w_tmp);
		ret.ww[i] = (add_c - mul_c)/(2.f*eps);
	}
	return ret;
}

weight learn(weight w, weight dw, float rate)
{
	for(int i=0 ; i<weight_count ; ++i){
		w.ww[i] -= rate*dw.ww[i];
	}
	return w;
}

void dump(weight w)
{
	for(int i=0 ; i<weight_count ; ++i){
		printf("%f ",w.ww[i]);
	}
	printf("\n");
}

int main(void)
{
	srand(69);
	set_rand_float();

	float eps  = 1e-3;
	float rate = 1e-2;

	for(int i = 0 ; i < 1000*1000 ; ++i){
		weight dw = partial_derivative(ws,eps);
		ws = learn(ws,dw,rate);
		dump(ws);
		printf("loss : %f\n",cost(ws));
	}

	for(size_t i = 0; i<2;++i){
		for(size_t j = 0; j<2;++j){
			printf("%zu | %zu | %f \n",i,j,forward(ws,i,j));
		}
	}

	return 0;
}
