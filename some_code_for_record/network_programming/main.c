/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 26 Mar 2025 09:35:10 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netdb.h>
#include"rio.h"
#define MAXLINE 1024

void unix_error(char *msg){
	fprintf(stderr,"%s : %s\n",msg,strerror(errno));
	exit(0);
}

int main(int argc,char **argv){
	if(argc != 2){
		unix_error("fomat ./myhostinfo www.website.com");
	}
	struct addrinfo hints;
	struct addrinfo *res,*cur;
	memset(&hints,0,sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(argv[1],NULL,&hints,&res);

	char hostName[MAXLINE];
	for(cur = res; cur != NULL ; cur = cur->ai_next){
		getnameinfo(cur->ai_addr,cur->ai_addrlen,hostName,sizeof(hostName),NULL,0,NI_NUMERICHOST);
		printf("canon-name : %s , host name : %s\n",cur->ai_canonname,hostName);
	}
	freeaddrinfo(res);
	exit(0);
}

