/*************************************************************************
	> File Name: rio.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 25 Mar 2025 10:15:33 AM CST
 ************************************************************************/

#ifndef __RIO_H__
#define __RIO_H__
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#define RIO_BUFSIZE 8192
typedef struct{
	int rio_fd;
	int rio_cnt;
	char *rio_bufptr;
	char rio_buf[RIO_BUFSIZE];
}rio_t;

#endif
