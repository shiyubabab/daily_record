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
void mat_reshape(Mat *m, size_t rows, size_t cols);

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
void nn_backprop(NN nn, NN gn, Mat ti, Mat to);
void nn_learn(NN nn, NN gn, float rate);

/***************
*  CNN DEFINE  *
****************/
typedef struct{
	size_t rows;
	size_t cols;
	size_t channel;
	size_t stride;
	size_t padding;
	Mat *ws;
	Mat m;
} Knl;

typedef struct{
	size_t rows;
	size_t cols;
	size_t channel;
	Mat *ds;
} Tdata;

#define OUT_H(td,knl) ((td).rows + 2 * (knl).padding - (knl).rows) / (knl).stride + 1
#define OUT_W(td,knl) ((td).cols + 2 * (knl).padding - (knl).cols) / (knl).stride + 1

Knl cnn_alloc(size_t rows, size_t cols, size_t channel, size_t stride, size_t padding);
void cnn_unfold(Knl knl);
void cnn_rand(Knl knl,size_t low, size_t height);

Tdata cnn_td_alloc(size_t rows, size_t cols, size_t channel);
Mat cnn_td_unfold(Tdata td, Knl knl);


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

void mat_reshape(Mat *m, size_t rows, size_t cols)
{
	NN_ASSERT(m->rows * m->cols == rows * cols);
	m->rows = rows;
	m->cols = cols;
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

void nn_backprop(NN nn, NN gn, Mat ti, Mat to)
{
	NN_ASSERT(ti.rows == to.rows);
	size_t n = ti.rows;

	for(size_t i = 0;i<gn.count;++i){
		mat_fill(gn.ws[i],0);
		mat_fill(gn.bs[i],0);
	}

	for(size_t i = 0;i < n;++i){
		mat_copy(NN_INPUT(nn),mat_row(ti,i));
		nn_forward(nn);

		for(size_t l = 0; l<=nn.count; ++l) mat_fill(gn.as[l],0);

		size_t L = nn.count;
		for(size_t j = 0; j < nn.as[L].cols; ++j){
			float a = MAT_AT(nn.as[L],0,j);
			float y = MAT_AT(to,i,j);

			MAT_AT(gn.as[L],0,j) = (a - y) * sigmoid_derf(a);
		}

		for(size_t l = L; l > 0; --l){
			for(size_t j = 0;j<nn.as[l].cols; ++j){
				float delta = MAT_AT(gn.as[l],0,j);
				for(size_t k = 0;k<nn.as[l-1].cols;++k){
					MAT_AT(gn.ws[l-1],k,j) += MAT_AT(nn.as[l-1],0,k) * delta;
				}
				MAT_AT(gn.bs[l-1],0,j) += delta;
			}

			if(l > 1) {
				for(size_t k = 0; k < nn.as[l-1].cols; ++k){
					float next_grad = 0;
					for(size_t j = 0; j < nn.as[l].cols; ++j){
						next_grad += MAT_AT(gn.as[l],0,j) * MAT_AT(nn.ws[l-1], k, j);
					}
					float a_prev = MAT_AT(nn.as[l-1], 0, k);
					MAT_AT(gn.as[l-1], 0, k) = next_grad * sigmoid_derf(a_prev);
				}
			}
		}
	}
	for(size_t i = 0; i < gn.count; ++i){
		for(size_t j = 0; j < gn.ws[i].rows * gn.ws[i].cols; ++j) gn.ws[i].es[j] /= n;
		for(size_t j = 0; j < gn.bs[i].rows * gn.bs[i].cols; ++j) gn.bs[i].es[j] /= n;
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


/********
*  CNN  *
*********/

Knl cnn_alloc(size_t rows, size_t cols, size_t channel, size_t stride, size_t padding)
{
	NN_ASSERT(rows > 0);
	NN_ASSERT(cols > 0);
	NN_ASSERT(channel > 0);

	Knl ret;
	ret.rows = rows;
	ret.cols = cols;
	ret.channel = channel;
	ret.stride = stride;
	ret.padding = padding;

	ret.ws = NN_MALLOC(sizeof(Mat)*ret.channel);
	for(size_t i = 0; i<ret.channel ;++i){
		ret.ws[i] = mat_alloc(ret.rows, ret.cols);
	}
	ret.m = mat_alloc(ret.rows * ret.cols * ret.channel , 1);

	return ret;
}

void cnn_rand(Knl knl,size_t low, size_t height)
{
	NN_ASSERT(low < height);
	for(size_t c = 0; c < knl.channel; ++c){
		mat_rand(knl.ws[c], low, height);
	}
}

Tdata cnn_td_alloc(size_t rows, size_t cols, size_t channel)
{
	NN_ASSERT(rows > 0);
	NN_ASSERT(cols > 0);
	NN_ASSERT(channel > 0);

	Tdata td;
	td.rows = rows;
	td.cols = cols;
	td.channel = channel;
	td.ds = NN_MALLOC(sizeof(Mat)*td.channel);
	for(size_t i = 0; i<td.channel ;++i){
		td.ds[i] = mat_alloc(td.rows, td.cols);
	}

	return td;
}

void cnn_unfold(Knl knl)
{
	NN_ASSERT(knl.rows > 0);
	NN_ASSERT(knl.cols > 0);
	NN_ASSERT(knl.channel > 0);

	size_t kc = knl.channel;
	size_t mrow = 0;
	for(size_t c = 0; c < kc; ++c){

		for(size_t i = 0; i<knl.rows; ++i){
			for(size_t j = 0; j<knl.cols; ++j){
				MAT_AT(knl.m,mrow,0) = MAT_AT(knl.ws[c],i,j);
				mrow++;
			}
		}
	}
}

Mat cnn_td_unfold(Tdata td, Knl knl)
{
	size_t out_h = OUT_H(td,knl);
	size_t out_w = OUT_W(td,knl);

	size_t dh = out_h * out_w; 
	size_t dw = knl.rows * knl.cols * td.channel;

	Mat ret = mat_alloc(dh,dw);

	size_t curr_row = 0;
	for(int y = -(int)knl.padding; y <= (int)(td.rows + knl.padding - knl.rows); y += (int)knl.stride){
		for(int x = -(int)knl.padding; x <= (int)(td.cols + knl.padding - knl.cols); x += (int)knl.stride){

			size_t curr_col = 0;
			for(size_t c = 0; c < td.channel; ++c){
				for(size_t ky = 0; ky < knl.rows; ++ky){
					for(size_t kx = 0; kx < knl.cols; ++kx){
						int iy = y + (int)ky;
						int ix = x + (int)kx;

						float val = 0.f;
						if(iy >=0 && iy < (int)td.rows && ix>=0 && ix < (int)td.cols ){
							val = MAT_AT(td.ds[c],iy,ix);
						}

						MAT_AT(ret,curr_row,curr_col) = val;
						curr_col++;
					}
				}
			}
			curr_row++;
		}
	}
	return ret;
}


#endif //NN_IMPLEMENTATION
