/*************************************************************************
	> File Name: rio.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 26 Mar 2025 09:39:39 AM CST
 ************************************************************************/

#ifndef __RIO_H__
#define __RIO_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#define RIO_BUFSIZE 8192

typedef struct{
	int rio_fd;
	int rio_cnt;
	char *rio_bufptr;
	char rio_buf[RIO_BUFSIZE];

}rio_t;

void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_read(rio_t *rp,cahr *usrbuf,size_t n);

#endif
