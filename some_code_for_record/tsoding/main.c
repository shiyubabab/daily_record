/*************************************************************************
	> File Name: main.c
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
	{1, 1, 1},
};

sample * train = or_train;
#define train_count 4

float sigmoidf(float x)
{
	return 1.0f/(1.0f + expf(-x));
}

float rand_float(void)
{
	return (float) rand() / (float) RAND_MAX;
}

float cost(float w1, float w2, float b)
{
	float result;
	for(int i = 0; i < train_count; ++i){
		float x1 = train[i][0];
		float x2 = train[i][1];

		float y= sigmoidf(w1*x1 + w2*x2 + b);
		float d = train[i][2] - y;
		result += d * d;
	}
	result /= train_count;
	return result;
}

int main(void){
	//srand(69);
	float w1 = 29.f;
	float w2 = 10.f;
	float b  = -4.7f;

	float eps  = 1e-1;
	float rate = 1e-1;

	for(int i = 0 ; i < 100*1000 ; ++i){
		float c = cost(w1,w2,b);
		printf("w1 = %f, w2 = %f, b = %f, cost = %f \n",w1,w2,b,c);
		float dw1 = (cost(w1+eps,w2,b) - c)/eps;
		float dw2 = (cost(w1,w2+eps,b) - c)/eps;
		float db  = (cost(w1,w2,b+eps) - c)/eps;

		w1 -= rate*dw1;
		w2 -= rate*dw2;
		b  -= rate*db;
	}

	for(size_t i = 0; i<2;++i){
		for(size_t j = 0; j<2;++j){
			printf("%zu | %zu | %f \n",i,j,sigmoidf(w1*i+w2*j+b));
		}
	}

	return 0;
}
