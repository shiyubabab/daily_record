/*************************************************************************
	> File Name: mlxh_palloc.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 13 Oct 2025 02:10:41 PM CST
 ************************************************************************/

#include <stdio.h>
#include "mlxh_palloc.h"

#define MLXH_POOL_ALIGNMENT		16

mlxh_pool_t *
mlxh_create_pool(size_t size)
{
	mlxh_pool_t *p;
	
	p = mlxh_memalign(MLXH_POOL_ALIGNMENT,size);
	if(p == NULL){
		printf("Errer: mlxh_memalign is error!!! \n");
		return NULL;
	}

	p->d.last = (u_char *) p + sizeof(mlxh_pool_t);
	p->d.end = (u_char *) p + size;
	p->d.next = NULL;
	p->d.failed = 0;

	size = size - sizeof(mlxh_pool_t);
	p->max = (size < MLXH_MAX_ALLOC_FROM_POOL) ? size : MLXH_MAX_ALLOC_FROM_POOL;

	p->current = p;
	p->chain = NULL;
	p->large = NULL;
	p->cleanup = NULL;

	return p;
}

void handler_master(void *data){
	mlxh_pool_t *data_ = (mlxh_pool_t *)data;
	printf("last : %p \n",data_->d.last);
}

int init_func(void){
	struct mlxh_pool_cleanup_pt	 mlxh_pool_cleanup;
	mlxh_pool_cleanup{
		.handler = handler_master,
	}

	return 0;
}
