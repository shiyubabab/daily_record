/*************************************************************************
	> File Name: common.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 17 Feb 2026 08:35:55 PM CST
 ************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678
#define MAX_SIZE 1024

struct shared_data {
	int id;
	char message[MAX_SIZE];
	int update_flag;
};

static struct sembuf sem_lock	= { 0, -1, SEM_UNDO};
static struct sembuf sem_unlock = { 0,	1, SEM_UNDO};

#endif
