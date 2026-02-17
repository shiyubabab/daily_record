/*************************************************************************
	> File Name: bitmap.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 17 Feb 2026 03:23:25 PM CST
 ************************************************************************/

#ifndef __BITMAP_H__
#define __BITMAP_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
	uint8_t *bits;
	size_t bit_count;
} Bitmap;

Bitmap * bitmap_create(size_t n);
void bitmap_set(Bitmap *bm,size_t k);
void bitmap_clean(Bitmap *bm, size_t k);
int bitmap_test(Bitmap *bm, size_t k);
void bitmap_free(Bitmap *bm);

#endif

#ifdef BITMAP_IMPLEMENTATION
Bitmap * bitmap_create(size_t n)
{
	Bitmap *bm = (Bitmap *) malloc(sizeof(Bitmap));
	bm->bit_count = (n + 7) / 8;
	bm->bits = (uint8_t *)calloc(bm->bit_count,sizeof(uint8_t));
	return bm;
}

void bitmap_set(Bitmap *bm,size_t k)
{
	bm->bits[k>>3] |= (1 << (k & 0x07));
}

void bitmap_clean(Bitmap *bm, size_t k)
{
	bm->bits[k>>3] &= ~(1 << (k & 0x07));
}

int bitmap_test(Bitmap *bm, size_t k)
{
	return (bm->bits[k>>3] & (1<< (k & 0x07))) != 0;
}

void bitmap_free(Bitmap *bm)
{
	free(bm->bits);
	free(bm);
}

#endif

