/*************************************************************************
	> File Name: lock.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 24 Mar 2026 02:57:37 PM CST
 ************************************************************************/

#ifndef __LOCK_H__
#define __LOCK_H__
#include <sched.h>
#include <pthread.h>

enum lock_result {
	UNLOCK = 0,
	LOCK
};

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
	while(LOCK == xchg(mutex))
		sched_yield();
}

static inline void unlock(int * mutex)
{
	*mutex = UNLOCK;
}

#endif //__LOCK_H__
