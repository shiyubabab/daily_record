/*************************************************************************
	> File Name: cnn.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 13 Apr 2026 09:19:27 AM CST
 ************************************************************************/

#include <stdio.h>
#define NN_IMPLEMENTATION
#include "nn.h"

#include <time.h>

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
	srand(time(NULL));
	Tdata td = cnn_td_alloc(4,3,3);

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

	CnnLayer layer_in = cnn_layer_create(4,3,3,3,16,1,1);
	cnn_layer_forward(layer_in, td);

	CnnLayer layer_2 = cnn_layer_create(4,3,16,3,3,1,1);
	cnn_layer_forward(layer_2, layer_in.output);

	MAT_DUMP(layer_in.dot_result);

	LAYER_DUMP(layer_in);
	LAYER_DUMP(layer_2);

	return 0;
}
