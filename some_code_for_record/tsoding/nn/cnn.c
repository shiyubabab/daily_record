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
	1, 143, 0, 0,
	1, 0, 1432, 143, 
	0, 1, 1, 2432,
	0, 0, 0, 4432,

	3, 1, 0, 5,
	1, 0, 1, 6,
	0, 1, 1, 7,
	0, 0, 0, 8,

	2, 1, 0, 9,
	1, 0, 1, 2,
	0, 1, 1, 8,
	0, 0, 0, 0
};

float data_tag[] = {
	109, 209,
	1392, 189,

	293, 39,
	484903, 84930,

	380, 19430,
	43, 43430,

	98, 19430,
	906798, 90008,

	232, 143,
	1321, 3123,

	432, 321,
	321, 321,
};

int main(void){
	srand(time(NULL));

	Tdata ti = cnn_td_alloc(4, 4, 3);
	for(size_t c = 0; c < ti.channel; ++c){

		for(size_t i = 0; i < ti.rows; ++i){
			for(size_t j = 0; j < ti.cols; ++j){
				size_t iy = i + c*ti.rows;
				MAT_AT(ti.ds[c],i,j) = data[iy*3+j];
				printf("%f ",MAT_AT(ti.ds[c],i,j));
			}
			printf("\n");
		}
		printf("\n");
	}

	Tdata to = cnn_td_alloc(2, 2, 6);
	for(size_t c = 0; c < to.channel; ++c){

		for(size_t i = 0; i < to.rows; ++i){
			for(size_t j = 0; j < to.cols; ++j){
				size_t iy = i + c*to.rows;
				MAT_AT(to.ds[c],i,j) = data[iy*3+j];
				printf("%f ",MAT_AT(to.ds[c],i,j));
			}
			printf("\n");
		}
		printf("\n");
	}

	CnnLayer layer_in = cnn_layer_create(4,4,3,3,6,2,1);
	cnn_layer_forward(layer_in, ti);

	CnnLayer layer_2 = cnn_layer_create(2,2,6,3,6,1,1);
	cnn_layer_forward(layer_2, layer_in.output);

	for(size_t c = 0;c<to.channel;++c){
		for(size_t i = 0;i<to.rows;++i){
			for(size_t j = 0;j<to.cols;++j){
				MAT_AT(to.ds[c],i,j) -= MAT_AT(layer_in.output.ds[c],i,j);
			}
		}
	}


	cnn_layer_backprop(layer_2,to);
	cnn_layer_backprop(layer_in,layer_2.ginput);

	LAYER_DUMP(layer_in);
	LAYER_DUMP(layer_2);

#if 0
	CnnLayer layer_2 = cnn_layer_create(4,3,3,3,3,1,1);
	cnn_layer_forward(layer_2, layer_in.output);

	for(size_t c = 0;c<td.channel;++c){
		for(size_t i = 0;i<td.rows;++i){
			for(size_t j = 0;j<td.cols;++j){
				MAT_AT(td.ds[c],i,j) -= MAT_AT(layer_2.output.ds[c],i,j);
			}
		}
	}

	cnn_layer_backprop(layer_2,td);
	cnn_layer_backprop(layer_in,layer_2.ginput);

	LAYER_DUMP(layer_in);
	LAYER_DUMP(layer_2);

	MAT_DUMP(layer_2.gbias);
	MAT_DUMP(layer_2.gknl.ws[0]);
	MAT_DUMP(layer_2.gknl.ws[1]);
	MAT_DUMP(layer_2.gknl.ws[2]);
#endif

	return 0;
}
