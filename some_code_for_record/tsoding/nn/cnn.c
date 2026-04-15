/*************************************************************************
	> File Name: cnn.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 13 Apr 2026 09:19:27 AM CST
 ************************************************************************/

#include <stdio.h>
#define NN_IMPLEMENTATION
#include "nn.h"

float data[] = {
	1, 1, 0,
	1, 0, 1,
	0, 1, 1,
	0, 0, 0,

	3, 1, 0,
	1, 0, 1,
	0, 1, 1,
	0, 0, 0,

	2, 1, 0,
	1, 0, 1,
	0, 1, 1,
	0, 0, 0,
};

int main(void){
	Knl knl  = cnn_alloc(3,3,3,1,1);
	Tdata td = cnn_td_alloc(4,3,3);

	cnn_rand(knl,0,1);

	for(size_t c = 0; c < td.channel; ++c){

		for(size_t i = 0; i < td.rows; ++i){
			for(size_t j = 0; j < td.cols; ++j){
				size_t iy = i + c*td.rows;
				MAT_AT(td.ds[c],i,j) = data[iy*3+j];
				printf("%f ",MAT_AT(td.ds[c],i,j));
			}
			printf("\n");
		}
		printf("\n");
	}

	cnn_unfold(knl);
	Mat td_unfold = cnn_td_unfold(td,knl);

	size_t out_h = OUT_H(td,knl);
	size_t out_w = OUT_W(td,knl);
	Mat result = mat_alloc(out_h * out_w,1);
	mat_dot(result,td_unfold,knl.m);
	mat_reshape(&result, td.rows, td.cols);

	MAT_DUMP(result);

	return 0;
}
