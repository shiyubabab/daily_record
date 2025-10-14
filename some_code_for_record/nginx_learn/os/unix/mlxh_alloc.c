/*************************************************************************
	> File Name: mlxh_alloc.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 13 Oct 2025 01:58:26 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void *
mlxh_memalign(size_t alignment, size_t size)
{
	void *p;
	int err;

	err = posix_memalign(&p,alignment,size);
	if(err){
		printf("posix_memalign(%uz,%uz) failed\n",alignment,size);
		p = NULL;
	}
	printf("posix_memalign: %p:%uz @%uz \n",p,size,alignment);
	return p;
}

