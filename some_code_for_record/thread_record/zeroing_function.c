/*************************************************************************
	> File Name: zeroing_function.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 07 Mar 2025 10:47:44 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include<string.h>

#define NTHREADS 10

typedef struct bzeroparams{
	unsigned char *buffer;
	int length;
	int number;
}bzeroparams_t;

static void *go(bzeroparams_t *p){
	memset(p->buffer,0,p->length);
	printf("It is thread[%d] buffer is %s\n",p->number,p->buffer);
	int *valueExit = (int *)malloc(sizeof(int));
	*valueExit = p->length;
	return valueExit;
}

static void blockzero(unsigned char *p,int length){
	int i;
	pthread_t threads[NTHREADS];
	bzeroparams_t params[NTHREADS];
	assert((length % NTHREADS)==0);
	for(i = 0; i < NTHREADS; i++){
		params[i].buffer = p + i * length/NTHREADS;
		params[i].length = length/NTHREADS;
		params[i].number = i;
		pthread_create(&(threads[i]),NULL, &go, (void *)(params+i));
	}
	for(i = 0; i < NTHREADS; i++){
		int **valueRet = (int **)malloc(sizeof(int *));
		pthread_join(threads[i],(void **)valueRet);
		printf("It is thread[%d] return value is %ld\n",i,**valueRet);
	}

}

int main(void){
	unsigned char p = 'a';
	blockzero(&p,50);
	return 0;
}

