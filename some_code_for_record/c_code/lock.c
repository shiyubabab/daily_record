/*************************************************************************
	> File Name: lock.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 21 Sep 2025 02:25:01 PM CST
 ************************************************************************/

#include<stdio.h>

/* 多线城、多进程，锁，原子操作
   1、锁的本质是一个“内存整数”，它的读写都是“原子操作”
			pthread_mutex_t 互斥锁
			spinlock 自旋锁，多个进程之间保护“共享数据”

			共享内存 2个进程 spinlock
			2个线程的内存本身就是共享的 pthread_mutex_t

			mutex 内核中受限制，在进程上下文中可以使用
			中断irq，软中断softirq，tasklete，这些中断上下文中不能加普通锁，会导致操作系统睡眠
	
	2、g_count = 0, g_count = 1, 原子操作
			mov $0, (%rax) 原子操作

			inc (%rax) 不是原子操作
				mov (%rax) ,  %edx
				add $1	   .  %edx
				mov %edx   , (%rax)

			add $1 , (%rax) 不是原子操作
	3、CPU “Linux 调度系统，”两条（原子）指令“之间可以打断进程的运行
	4、锁，内存的整数，一定是 通过它的“指针“访问！！！！！
			指针的读写，一定是访问的内存！！！！！
			变量名的读写，编译器优化之后，可能是读写的寄存器！！！！！
			内存可以在多个线程之间共享，寄存器隶属于单个CPU

	5、futex，快速 用户态 互斥区
		获取锁失败
		线程告诉内核 我去休眠，等有其他线程释放锁的时候，你再唤醒我
		内核等其他线程释放锁，唤醒它
		

 */

/*
	mutex 指针，对应所的内存
	0表示解锁, 1表示加锁

	1、r = 1
	2、原子交换 *mutex <-----> r
	3、r == 0 第2步之前，锁是解开的，加锁成功！
	4、r == 1 第2步之前，有其他线程拿着这个锁，加锁失败！
	5、xchg指令，intel CPU,交换一个内存数据 和 寄存器的值
			mov  $1  , %eax
			xchg %eax,(%rdi)
*/

static inline int xchg(int * mutex)
{

	int r;
	asm volatile(
			"xchg %0,(%1)"
			:"=r"(r)
			:"r"(mutex),"0"(1)
			:
	);
	return r;
}

static inline void lock(int * mutex)
{
	while(1 == xchg(mutex)) //自旋
		pthread_yield(); //让出cpu时间片，让其他的线程运行

}

static inline void unlock(int *mutex)
{
	*mutex = 0; // mov $0,(%rdi)
}

void *thread(void * arg)
{
	int i;
	for(i = 0; i< N; i++){
		lock(&g_mutex);
		g_count++;
		unlock(&g_mutex);
	}
}

int main()
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

