/*************************************************************************
	> File Name: hello_world.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 06 Mar 2025 11:32:30 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 10
static pthread_t threads[NTHREADS];
static void *go(void *n){
	printf("Hello from thread %d\n",n);
	int *ret = (int *)malloc(sizeof(int));
	*ret = 100+(int)n;
	return ret;
}

int main(void){
	int i;
	long exitValue;

	for(i = 0;i<NTHREADS;i++){
		pthread_create(&(threads[i]),NULL,&go, (void *)i);
	}
	for(i = 0;i<NTHREADS;i++){
		int **exitValue = (int **)malloc(sizeof(int *));
		pthread_join(threads[i],(void **)exitValue);
		printf("Thread %d returned with %ld \n",i,**exitValue);
	}
	printf("Main thread done.\n");
	return 0;
}
