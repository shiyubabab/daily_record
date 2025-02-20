#ifndef __HEAP_H__
#define	__HEAP_H__

/*
	| Header | Payload (allocated block only) | Padding | Footer |
	  4byte				malloc					 对齐	   4byte

	Header ----> | block size | 0 | 0 | a |	32bit
					29bit
	GET_SIZE(p) (GET(p) & -0x7)	---------> |111111111...|001|
											 29bit		 3bit
*/		

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)

#define MAX(x,y) ((x)>(y)?(x):(y))

#define PACK(size , alloc) ((size)|(alloc))

#define GET(p)	(*(unsigned int *)(p))
#define PUT(p,val)	(*(unsigned int *)(p)) = (val)


#define GET_SIZE(p) (GET(p) & -0x7)	
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char*)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char*)(bp) - DSIZE)))

#endif
