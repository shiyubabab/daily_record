/*************************************************************************
	> File Name: data.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 12 Oct 2025 02:37:43 PM CST
 ************************************************************************/

#ifndef __DATA_H__
#define __DATA_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef intptr_t	mlxh_int_t;
typedef uintptr_t	mlxh_uint_t;

typedef struct {
	size_t	len;
	u_char	*data;
}mlxh_string_t;

typedef struct mlxh_pool_s	mlxh_pool_t;

struct mlxh_pool_s{
	mlxh_pool_t *current;
};


typedef struct mlxh_list_part_s {
	void				*elts;
	mlxh_uint_t			nelts;
	struct mlxh_list_part_s	*next;
}mlxh_list_part_t;

typedef struct {
	mlxh_list_part_t	*last;
	mlxh_list_part_t	part;
	size_t				size;
	mlxh_uint_t			nalloc;
	mlxh_pool_t			*pool;
}mlxh_list_t;

#endif
