#include<stdio.h>
#include<stdlib.h>
#include"heap.h"
#define MAX_HEAP (20*(1<<20)) //20MB


static char *mem_heap;
static char *mem_brk;
static char *mem_max_addr;

void mem_init(void){
	mem_heap = (char *)malloc(MAX_HEAP);
	mem_brk = (char *)mem_heap;
	mem_max_addr = (char *)(mem_heap + MAX_HEAP);
}

void *mem_sbrk(int incr){
	char *old_brk = mem_brk;
	if((incr<0)||((mem_brk+incr)>mem_max_addr)){
		printf("ERROR: mem_sbrk failed. Ran out of memory...\n");
		return (void *)-1;
	}
	mem_brk+=incr;
	return (void *)old_brk;
}

int mm_init(void){
	if((heap_listp=mem_sbrk(4*WSIZE))==(void *)-1)
		return -1;
	PUT(heap_listp + (1*WSIZE),PACK(DSIZE,1));
	PUT(heap_listp + (2*WSIZE),PACK(DSIZE,1));
	PUT(heap_listp + (3*WSIZE),PACK(0,1));
	heap+=(2*WSIZE);
	return 0;
}

static void *coalesce(void *bp){
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if(prev_alloc && next_alloc){
		return bp;
	}

	else if(prev_alloc && !next_alloc){
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp),PACK(size,0));
		PUT(FTRP(bp),PACK(size,0)); 
		//用头部的GET_SIZE去做，此时的size已经被修改，所以直接访问到脚部.
	}
	
	else if(!prev_alloc && next_alloc){
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp),PACK(size,0));
		PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
		bp = PREV_BLKP(bp);
	}
	
	else{
		size += GET_SIZE(HDRP(NEXT_BLKP(bp))) 
			 +  GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
		PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
		bp = PREV_BLKP(bp);
	}
	return bp;
}
