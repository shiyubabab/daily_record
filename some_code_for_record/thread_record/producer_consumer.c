/*************************************************************************
	> File Name: producer_consumer.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Mon 24 Mar 2025 03:38:08 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

typedef struct sbuf{
	int *buf;
	int n;

	int rear;
	int front;

	sem_t mutex;
	sem_t slots;
	sem_t items;
}sbuf_t;

void sbuf_init(sbuf_t *sp,int n){
	sp->n     = n;
	sp->buf   = calloc(n,sizeof(int));
	sp->rear  = 0;
	sp->front = 0;

	sem_init(&sp->mutex,0,1);
	sem_init(&sp->slots,0,n);
	sem_init(&sp->items,0,0);

}

void sbuf_deinit(sbuf_t *sp){
	free(sp->buf);
}

void sbuf_insert(sbuf_t *sp,int item){
	sem_wait(&sp->slots);
	sem_wait(&sp->mutex);

	sp->buf[(++sp->rear)%(sp->n)] = item;

	sem_post(&sp->mutex);
	sem_post(&sp->items);

}

int sbuf_remove(sbuf_t *sp){
	int item;
	sem_wait(&sp->items);
	sem_wait(&sp->mutex);

	item = sp->buf[(++sp->front)%(sp->n)];

	sem_post(&sp->mutex);
	sem_post(&sp->slots);

	return item;
}

int main(void){
	sbuf_t test;
	sbuf_init(&test,5);
	sbuf_insert(&test,1);
	int item = sbuf_remove(&test);
	printf("item is [%d]\n",item);
	sbuf_deinit(&test);

}
