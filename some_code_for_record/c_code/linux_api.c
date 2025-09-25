/*************************************************************************
	> File Name: linux_api.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 23 Sep 2025 09:23:29 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* 1、Linux API（syscall）, 系统提供给用户的一些编程接口（函数）
   2、内存管理、进程管理、文件系统、设备管理、网络系统
   3、内存管理：brk() 堆内存 --> sbrk() --> malloc() / free() ，mmap() 文件映射
   4、进程管理：fork() / sched_yield() / wait() / exit(), pthread_create() / pthread_join() / pthread_yield() / pthread_mutex_lock()
				Linux 内核在数据结构上不区分 进程和线程，区别在于 内存的共享模式
   5、文件系统：c语言之父的设计模式："一切都是文件"，unix/Linux	
				vfs : virtual file system. 函数指针 ==== C++的虚基类，OOP
				内存文件系统：/proc /sys /tmp
				设备文件系统：/dev
				硬盘文件系统：/		/home /boot
				网络socket: 文件系统 --> 网络子系统
				文件API ： open() / close() / write() / fcntl() / mmap()
				ipc：共享内存，socketpair() localsocket
 */

int xchg(volatile int *p){
	int r = 1;
	asm volatile("xchg %1,%0"
			:"=m"(*p),"=r"(r)
			:"1"(r)
			:);
	return r;
}

int spin_trylock(volatile int * p){
	int n = 0;
	while(1==xchg(p)){
		n++;
		if(n>1000)
			return 0;
	}
	return 1;
}

void spin_lock(volatile int * p){
	while(1==xchg(p))
		sched_yield();
}


void spin_unlock(volatile int *p){
	*p = 0;
}

typedef struct{
	volatile int lock;
	volatile int count;
}shm_t;


int main(void){
	int fd = shm_open("shm_obj",O_RDWR|O_CREAT|O_TRUNC,0666);
	if(fd < 0){
		printf("shm_open error,errno:%d \n",errno);
		return -1;
	}

	if(ftruncate(fd,sizeof(shm_t))<0){
		printf("ftruncate error, errno %d\n",errno);
		return -1;
	}

	shm_t* shm = mmap(NULL,sizeof(shm_t),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(MAP_FAILED == shm){
		printf("mmap error,errno:%d\n",errno);
		return -1;
	}

	shm->lock  = 0;
	shm->count = 0;
	int n = 0;
//多进程 程序，需要在父子进程之间共享的数据，全在fork()之前申请
	pid_t cpid = fork();
	if(-1 == cpid){
		printf("fork error\n");
		return -1;
	}

	if(0 == cpid){
		while(n<1000*1000){
			spin_lock(&shm->lock);
			shm->count++;
			spin_unlock(&shm->lock);
			n++;
		}

		exit(0);
	}else{
		while(n<1000*1000){
			spin_lock(&shm->lock);
			shm->count++;
			spin_unlock(&shm->lock);
			n++;
		}

		int status;
		pid_t pid = wait(&status); //等待子进程退出
		assert(pid == cpid);
		printf("shm->count : %d \n",shm->count);
	}

	return 0;
}

