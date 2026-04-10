/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 24 Mar 2026 03:08:09 PM CST
 ************************************************************************/

#include <stdio.h>
#include "lock.h"
#define N 1024

int g_mutex = UNLOCK;
int g_count = 0;

void * thread(void * arg)
{
	int i;
	for(int i = 0; i<N; i++){
		lock(&g_mutex);
		g_count++;
		unlock(&g_mutex);
	}

}

int main(void)
{
	pthread_t tid;
	pthread_create(&tid,NULL,thread,NULL);

	int i;
	for(i = 0;i<N;i++){
		lock(&g_mutex);
		g_count++;
		unlock(&g_mutex);
	}

	pthread_join(tid,NULL);
	printf("g_count: %d\n",g_count);
	return 0;
}
