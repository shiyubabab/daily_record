/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 17 Feb 2026 03:47:34 PM CST
 ************************************************************************/

#define BITMAP_IMPLEMENTATION
#define RINGBUFFER_IMPLEMENTATION
#include "bitmap.h"
#include "ringbuffer.h"

int main(void)
{
	Bitmap * bm = bitmap_create(10);
	bitmap_set(bm,8);
	bitmap_set(bm,10);
	bitmap_set(bm,11);
	bitmap_set(bm,12);
	bitmap_set(bm,13);
	printf("bit map set 11 [%d]\n",bitmap_test(bm,11));
	bitmap_clean(bm,11);
	printf("bit map set 11 [%d]\n",bitmap_test(bm,11));
	bitmap_free(bm);


	FastRB rb ;
	rb_init(&rb);
	rb_write(&rb, 10);
	rb_write(&rb, 11);
	rb_write(&rb, 12);
	rb_write(&rb, 19);
	rb_write(&rb, 22);
	rb_write(&rb, 21);
	rb_write(&rb, 23);
	rb_write(&rb, 291);
	int data;
	rb_read(&rb,&data);
	printf("read the data is %d\n",data);
	rb_read(&rb,&data);
	printf("read the data is %d\n",data);
	rb_read(&rb,&data);
	printf("read the data is %d\n",data);
	rb_read(&rb,&data);
	printf("read the data is %d\n",data);
	rb_read(&rb,&data);
	printf("read the data is %d\n",data);

	return 0;
}
