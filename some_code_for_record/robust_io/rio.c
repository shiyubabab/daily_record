/*************************************************************************
	> File Name: rio.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 25 Mar 2025 10:37:45 AM CST
 ************************************************************************/

#include"rio.h"

//unbuffered
ssize_t rio_readn(int fd, void *usrbuf,size_t n){
	char *buf = usrbuf;
	size_t nread;
	size_t nleft = n;

	while(nleft > 0){
		nread = read(fd,buf,nleft);
		if(nread<0){
			if(errno == EINTR)
				nread = 0;
			else
				return -1;
		}else if(nread == 0)
			break;
		buf   += nread;
		nleft -= nread;
	}
	return (n-nleft);

}


ssize_t rio_writen(int fd, void *usrbuf,size_t n){
	char *buf = usrbuf;
	size_t nwrite;
	size_t nleft = n;

	while(nleft>0){
		if((nwrite = write(fd,buf,nleft))<=0){
			if(errno == 0)
				nwrite = 0;
			else
				return -1;
		}
		buf   += nwrite;
		nleft -= nwrite;
	}
	return (n-nleft);
}

//bufferen

void rio_readinitb(rio_t *rp,int fd){
	rp->rio_bufptr = rp->rio_buf;
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf,size_t n){
	while(rp->rio_cnt <= 0){
		//nothing to read,need to call systemcall.
		rp->rio_cnt = read(rp->rio_fd,rp->rio_buf,sizeof(rp->rio_buf));
		if(rp->rio_cnt<0){
			if(errno == 0)
				rp->rio_cnt = 0;
			else
				return -1;
		}else if(rp->rio_cnt == 0){
			break;
		}else{
			rp->rio_bufptr = rp->rio_buf;
		}
	}
	int cnt;
	cnt = (rp->rio_cnt < n) ? (rp->rio_cnt) : n;

	memcpy(usrbuf,rp->rio_bufptr,cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
	return cnt;
}

ssize_t rio_readnb(int fd, void *usrbuf,size_t n){
	char *buf = usrbuf;
	size_t nread;
	size_t nleft = n;

	while(nleft > 0){
		nread = rio_read(fd,buf,nleft);
		if(nread<0){
			if(errno == EINTR)
				nread = 0;
			else
				return -1;
		}else if(nread == 0)
			break;
		buf   += nread;
		nleft -= nread;
	}
	return (n-nleft);

}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen){
	int n,rc;	
	char c,*bufp = usrbuf;

	for(n = 1;n<maxlen;n++){
		if((rc = rio_read(rp,&c,1))==1){
			*bufp++ = c;
			if(c == '\n'){
				n++;
				break;
			}
		}else if(rc == 0){
			if(n==1)
				return 0;
			else
				break;
		}else{
			return -1;
		}
	}
	*bufp = 0;
	return n-1;
}
