/*************************************************************************
	> File Name: epoll_wait_demo.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 28 Sep 2025 03:59:52 PM CST
 ************************************************************************/

#include <stdio.h>
#include <signal.h>

typedef struct client_s client_t;

#define CLIENT_START		0
#define CLIENT_CONNECTING	1
#define CLIENT_CONNECTED	2
#define CLIENT_HTTP_SENT	3
#define CLIENT_CLOSE		4

struct client_s
{
	int status;

	int epfd;
	int fd;

	uint32_t events;

	int (*handler)(client* c);

	//网络 流量控制，定时器 rbtree
	char* write_buf;
	int write_buf_size;
	int write_buf_pos;

	int read_bytes;
	time_t time;
};

int client_handler(client_t* c)
{
	time_t t;
	time(&t);

	int ret = -1;

	switch(c->status){
		case CLIENT_START:
			ret = client_connect(c);
			break;
		case CLIENT_CONNECTING:
			ret = client_connect_check(c);
			break;
		case CLIENT_CONNECTED:
			ret = client_http_send(c);
			break;
		case CLIENT_HTTP_SENT:
			ret = client_http_recv(c);
			break;
		case CLIENT_CLOSE:
			ret = client_close(c);
			break;
		default:
			printf("default error\n");
			break;
	};

	if(-1 == ret){
		c->status = CLIENT_CLOSE;
		return -1;
	}

	if(t - c->time > 3 && CLIENT_HTTP_SENT == c->status){
		printf("stat c: %p,c->fd:%d,c->status:%d,received:%d\n",c,c->fd,c->status,c->read_bytes);

		c->status = CLIENT_CLOSE;
		time(&c->time);
	}
	return ret;
}


int main()
{
	signal(SIGPIPE,SIG_IGN); //网络编程，忽略 SIGPIPE
	/*
	   客户端与服务器有一个tcp链接，如若服务器将该链接关闭了，
	   这个时候客户端继续给服务端发送/接受信息就会有概率触发
	   SIGPIPE信号导致进程退出。
	 */
	int epfd = epoll_create(1024);
	if(epfd < 0){
		printf("epoll_create error,errno: %d\n",errno);
		return -1;
	}

#define N_CLIENTS 4
	client_t clients[N_CLIENTS] = {0};

	int i;
	for(i = 0;i<N_CLIENTS;i++){
		client_t* c = &clients[i];

		c->epfd   = epfd;
		c->status = CLIENT_START;
		c->fd	  = -1;
		c->events = 0;
		c->handler= client_handler;

		c->read_bytes = 0;
		time(&c->time);
	}

	while(1){
		struct epoll_event events[N_CLIENTS];

		int ret = epoll_wait(epfd,events,N_CLIENTS,500);
		if(ret < 0){
			printf("epoll_wait error,errno:%d\n",errno);
			return -1;
		}

		int i;
		for(i=0;i<ret;i++){
			struct epoll_event* ev = &events[i];

			client_t *c = ev->data.ptr;

			c->events = ev->events;

			int ret2 = client_handler(c);
			if(ret2 < 0){
				printf("client_handler error,ret2: %d\n",ret2);
			}
		}

		for(i=0;i<N_CLIENTS;i++){
			client_t* c = &clients[i];
			c->events	= 0;
			ret = client_handler(c);
			if(ret<0){
				printf("client_handler error2,ret:%d\n",ret);
			}
		}
		printf("\n");
	}

	return 0;
}
