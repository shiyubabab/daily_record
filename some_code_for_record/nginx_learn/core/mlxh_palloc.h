/*************************************************************************
	> File Name: mlxh_palloc.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 13 Oct 2025 02:12:29 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MLXH_MAX_ALLOC_FROM_POOL	(mlxh_pagesize - 1)

typedef void *				mlxh_buf_tag_t;
typedef struct mlxh_buf_s	mlxh_buf_t;
struct mlxh_buf_s {
	u_char				*pos;
	u_char				*last;
	u_char				*start;
	u_char				*end;
	mlxh_buf_tag_t		tag;
	mlxh_buf_t			*shadow;

	unsigned			temporary:1;
	unsigned			memory:1;
	unsigned			mmap:1;
	unsigned			recycled:1;
	unsigned			flush:1;
	unsigned			sync:1;
	unsigned			last_buf:1;
	unsigned			last_in_chain:1;
	unsigned			last_shadow:1;

	int					num;
};

typedef struct mlxh_chain_s	mlxh_chain_t;
struct mlxh_chain_s {
	mlxh_buf_t			*buf;
	mlxh_chain_t		*next;
};


typedef void (*mlxh_pool_cleanup_pt)(void *data);
typedef struct mlxh_pool_cleanup_s mlxh_pool_cleanup_t;
struct mlxh_pool_cleanup_s {
	mlxh_pool_cleanup_pt	handler;
	void					*data;
	mlxh_pool_cleanup_t		*next;
};

typedef struct mlxh_pool_large_s mlxh_pool_large_t;
struct mlxh_pool_large_s {
	mlxh_pool_large_t		*next;
	void					*alloc;
};


typedef struct mlxh_pool_s  mlxh_pool_t;
typedef struct{
	u_char				*last;
	u_char				*end;
	mlxh_pool_t			*next;
	//mlxh_uint_t			failed;
}mlxh_pool_data_t;

struct mlxh_pool_s {
	mlxh_pool_data_t	d;
	size_t				max;
	mlxh_chain_t		*chain;
	mlxh_pool_large_t	*large;
	mlxh_pool_cleanup_t	*cleanup;
};

mlxh_pool_t *mlxh_create_pool(size_t size);


