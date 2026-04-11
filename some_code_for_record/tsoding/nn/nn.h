/*************************************************************************
	> File Name: nn.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 08 Apr 2026 01:36:24 PM CST
 ************************************************************************/

#ifndef _NN_H_
#define _NN_H_
#include <math.h>

#ifndef NN_MALLOC
#include <stdlib.h>
#define NN_MALLOC malloc
#endif

#ifndef NN_ASSERT
#include <assert.h>
#define NN_ASSERT assert
#endif

#ifndef NN_PRINT
#include <stdio.h>
#define NN_INFO printf
#endif

/*********************
*	FUNCTION DEFINE	 *
**********************/

float sigmoidf(float x);
float sigmoid_derf(float x);
float rand_float(void);

/******************
*	MATRIX DEFINE *
*******************/

#define MAT_AT(m, i, j) (m).es[(i)*(m).stride + (j)]
typedef float (*activate_callback)(float);

typedef struct {
	size_t rows;
	size_t cols;
	size_t stride;
	float	*es;
} Mat;

Mat mat_alloc(size_t rows, size_t cols);
void mat_fill(Mat m, float x);
void mat_rand(Mat m, size_t low, size_t hight);
void mat_dump(Mat m,char *name,size_t padding);
#define MAT_DUMP(m) mat_dump(m,#m,0)
void mat_dot(Mat c, Mat a, Mat b);
void mat_add(Mat a, Mat b);
void mat_act(Mat m,activate_callback cb);
void mat_copy(Mat dst, Mat src);
Mat mat_row(Mat m, size_t row);

/***************
*	NN DEFINE  *
****************/

typedef struct {
	size_t count;
	Mat *ws;
	Mat *bs;
	Mat *as; //?? The amount of activations is count + 1
} NN;

#define NN_INPUT(nn) nn.as[0]
#define NN_OUTPUT(nn) (nn).as[(nn).count]
#define ARCH_LAYERS(x) sizeof(x) / sizeof(*x)
NN nn_alloc(size_t *arch, size_t arch_count);
void nn_dump(NN nn, char *name);
#define NN_DUMP(nn) nn_dump(nn,#nn)
void nn_forward(NN nn);
void nn_rand(NN nn, size_t low, size_t height);
float nn_cost(NN nn, Mat ti, Mat to);
void nn_finite_diff(NN nn, NN gn, float eps, Mat ti, Mat to);
void nn_learn(NN nn, NN gn, float rate);

#endif //_NN_H_

#ifdef NN_IMPLEMENTATION

/***************
*	FUNCTION   *
****************/

float sigmoidf(float x)
{
	return 1.f/(1.f + expf(-x));
}

float sigmoid_derf(float x)
{
	return x * (1.f - x);
}

float rand_float(void)
{
	return (float) rand() / (float) RAND_MAX;
}

/*************
*	MATRIX   *
**************/

Mat mat_alloc(size_t rows, size_t cols)
{
	Mat m;
	m.rows = rows;
	m.cols = cols;
	m.stride = cols;
	m.es = NN_MALLOC(sizeof(*m.es) * rows * cols);
	NN_ASSERT(m.es != NULL);
	return m;
}

void mat_fill(Mat m, float x)
{
	for(size_t i = 0;i<m.rows;++i){
		for(size_t j = 0;j<m.cols;++j){
			MAT_AT(m,i,j) = x;
		}
	}
}

void mat_rand(Mat m, size_t low, size_t hight)
{
	NN_ASSERT(low<hight);
	for(size_t i = 0;i<m.rows;++i){
		for(size_t j = 0;j<m.cols;++j){
			MAT_AT(m,i,j) = rand_float() * (hight - low) + low;
		}
	}
}

void mat_dump(Mat m,char *name,size_t padding)
{
	NN_INFO("%*s%s = [\n",(int)padding,"",name);
	for(size_t i = 0;i<m.rows;++i){
		NN_INFO("%*s    ",(int)padding,"");
		for(size_t j = 0;j<m.cols;++j){
			NN_INFO("%f ",MAT_AT(m,i,j));
		}
		NN_INFO("\n");
	}
	NN_INFO("%*s]\n",(int)padding,"");
}

void mat_dot(Mat c, Mat a, Mat b)
{	
	//	 i  x (k      k )	  j 
	// rows x (cols rows) x cols
	NN_ASSERT(a.cols == b.rows);
	NN_ASSERT(c.rows == a.rows);
	NN_ASSERT(c.cols == b.cols);

	for(size_t i = 0;i<c.rows;++i){
		for(size_t j = 0;j<c.cols;++j){
			MAT_AT(c,i,j) = 0.f;
			for(size_t k = 0;k<a.cols;++k){
				MAT_AT(c,i,j) += MAT_AT(a,i,k) * MAT_AT(b,k,j);
			}
		}
	}
}

void mat_add(Mat a, Mat b)
{
	NN_ASSERT(a.rows == b.rows);
	NN_ASSERT(a.cols == b.cols);

	for(size_t i = 0;i<a.rows;++i){
		for(size_t j = 0;j<a.cols;++j){
			MAT_AT(a,i,j) += MAT_AT(b,i,j);
		}
	}

}

void mat_act(Mat m, activate_callback cb)
{
	for(size_t i = 0;i<m.rows;++i){
		for(size_t j = 0;j<m.cols;++j){
			MAT_AT(m,i,j) = cb(MAT_AT(m,i,j));
		}
	}
}

void mat_copy(Mat dst, Mat src)
{
	NN_ASSERT(dst.rows == src.rows);
	NN_ASSERT(dst.cols == src.cols);

	for(size_t i = 0;i<dst.rows;++i){
		for(size_t j = 0;j<dst.cols;++j){
			MAT_AT(dst,i,j) = MAT_AT(src,i,j);
		}
	}
}

Mat mat_row(Mat m, size_t row)
{
	return (Mat) {
		.rows = 1,
		.cols = m.cols,
		.stride = m.stride,
		.es	  = &MAT_AT(m,row,0),
	};
}

/********
*	NN  *
*********/

/*
	arch {2, 2, 1}	  arch_count 3
	ws : 2
	bs : 2
	a  : 3
 */

NN nn_alloc(size_t *arch, size_t arch_count)
{
	NN_ASSERT(arch_count > 0);
	NN nn;
	nn.count = arch_count - 1;

	nn.ws = NN_MALLOC(sizeof(*nn.ws)*nn.count);
	NN_ASSERT(nn.ws != NULL);

	nn.bs = NN_MALLOC(sizeof(*nn.bs)*nn.count);
	NN_ASSERT(nn.bs != NULL);

	nn.as = NN_MALLOC(sizeof(*nn.as)*(nn.count+1));
	NN_ASSERT(nn.as != NULL);


	nn.as[0] = mat_alloc(1,arch[0]);
	for(size_t i = 1; i < arch_count; ++i){
		nn.ws[i-1] = mat_alloc(nn.as[i-1].cols,arch[i]);

		nn.bs[i-1] = mat_alloc(1,arch[i]);
		nn.as[i] = mat_alloc(1,arch[i]);
	}
	return nn;
}
void nn_dump(NN nn, char *name)
{
	NN_INFO("%s = [\n",name);
	size_t count = nn.count;
	char buf[256];
	for(size_t i = 0 ; i < count ; ++i){
		snprintf(buf,sizeof(buf),"ws[%zu]",i);
		mat_dump(nn.ws[i],buf,4);

		snprintf(buf,sizeof(buf),"bs[%zu]",i);
		mat_dump(nn.bs[i],buf,4);
	}
	NN_INFO("]\n");
}

void nn_forward(NN nn)
{
	for(size_t i = 0; i<nn.count; ++i){
		mat_dot(nn.as[i+1],nn.as[i],nn.ws[i]);
		mat_add(nn.as[i+1],nn.bs[i]);
		mat_act(nn.as[i+1],sigmoidf);
	}
}

void nn_rand(NN nn, size_t low, size_t height)
{
	NN_ASSERT(low < height);
	size_t count = nn.count;
	for(size_t i = 0; i < count; ++i){
		mat_rand(nn.ws[i],low,height);
		mat_rand(nn.bs[i],low,height);
	}
}

float nn_cost(NN nn, Mat ti, Mat to)
{
	NN_ASSERT(ti.rows == to.rows);
	NN_ASSERT(to.cols == NN_OUTPUT(nn).cols);

	size_t n = ti.rows;
	float result = 0.0f;
	for(size_t i = 0; i < n; ++i){
		mat_copy(NN_INPUT(nn),mat_row(ti,i));
		nn_forward(nn);

		size_t q = to.cols;
		for(size_t j = 0; j < q; ++j){
			float d = (float) (MAT_AT(NN_OUTPUT(nn),0,j) - MAT_AT(to,i,j));
			result += (float) d * d;
		}
	}
	return result/n;
}

void nn_finite_diff(NN nn, NN gn, float eps, Mat ti, Mat to)
{
	size_t count = nn.count;
	for(size_t c = 0; c < count; ++c) {

		Mat ws = nn.ws[c];
		Mat gws = gn.ws[c];
		for(size_t i = 0; i < ws.rows; ++i){
			for(size_t j = 0; j < ws.cols; ++j){
				float saved = MAT_AT(ws,i,j);

				MAT_AT(ws,i,j) = saved + eps;
				float a = nn_cost(nn,ti,to);

				MAT_AT(ws,i,j) = saved - eps;
				float b = nn_cost(nn,ti,to);

				MAT_AT(ws,i,j) = saved;
				MAT_AT(gws,i,j) = (a - b) / (2.f*eps);

			}
		}

		Mat bs = nn.bs[c];
		Mat gbs = gn.bs[c];
		for(size_t i = 0; i < bs.rows; ++i){
			for(size_t j = 0; j < bs.cols; ++j){
				float saved = MAT_AT(bs,i,j);

				MAT_AT(bs,i,j) = saved + eps;
				float a = nn_cost(nn,ti,to);

				MAT_AT(bs,i,j) = saved - eps;
				float b = nn_cost(nn,ti,to);

				MAT_AT(bs,i,j) = saved;
				MAT_AT(gbs,i,j) = (a - b) / (2.f*eps);

			}
		}
	}
}

void nn_learn(NN nn, NN gn, float rate)
{
	size_t count = nn.count;
	for(size_t c = 0; c < count; ++c) {

		Mat ws = nn.ws[c];
		Mat gws = gn.ws[c];
		for(size_t i = 0; i < ws.rows; ++i){
			for(size_t j = 0; j < ws.cols; ++j){
				MAT_AT(ws,i,j) -= rate*MAT_AT(gws,i,j);
			}
		}

		Mat bs = nn.bs[c];
		Mat gbs = gn.bs[c];
		for(size_t i = 0; i < bs.rows; ++i){
			for(size_t j = 0; j < bs.cols; ++j){
				MAT_AT(bs,i,j) -= rate*MAT_AT(gbs,i,j);
			}
		}
	}

}

#endif //NN_IMPLEMENTATION
