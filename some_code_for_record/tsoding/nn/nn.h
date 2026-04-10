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

/*
			(2)	 (2)
	(1,2)				(2,1)	（1,1)
			(2)	 (2)
	input	 w1+b1		w2+b2
arch {1,	  2,		 1}
 */
NN nn_alloc(size_t *arch, size_t arch_count);

#endif //_NN_H_

#ifdef NN_IMPLEMENTATION
float sigmoidf(float x)
{
	return 1.f/(1.f + expf(-x));
}

float rand_float(void)
{
	return (float) rand() / (float) RAND_MAX;
}

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


#endif //NN_IMPLEMENTATION
