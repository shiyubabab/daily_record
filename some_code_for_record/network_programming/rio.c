/*************************************************************************
	> File Name: rio.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 26 Mar 2025 09:43:09 AM CST
 ************************************************************************/

#include "rio.h"

void rio_readinitb(rio_t *rp,int fd){
	rp->rio_bufptr = rp->rio_buf;
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
}

ssize_t rio_read(rio_t *rp,char *usrbuf,size_t n){
	while(rp->rio_cnt <= 0){	/*nothing to read,we need call system call*/
		rp->rio_cnt = read(rp->rio_buf,rp->rio_fd,n);
		if(rp->rio_cnt < 0){
			if(errno == 0)
				rp->rio_cnt = 0;
			else
				return -1;
		}else if(rp->rio_cnt == 0){
			break;
		}else{
			rp->rio_bufptr += rp->rio_cnt;
		}
	}

	int cnt;
	cnt = (rp->rio_cnt > n) ? n : rp->rio_cnt;
	
	memcpy(usrbuf,rp->rio_buf,cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
}

