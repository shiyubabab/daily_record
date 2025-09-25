/*************************************************************************
	> File Name: socketpair_demo.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 23 Sep 2025 02:55:34 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

/*
   ipv4, AF_INET
   ipv6, AF_INET6
   ipc , AF_UNIX,AF_LOCAL

   tcp , SOCK_STREAM
   udp , SOCK_DGRAM

   应用层：		http rtmp rtsp  应用自己维护
   传输层：		tcp udp			内核协议栈 处理 传输层、网络层、数据链路层
   网络层：		ip
   数据链路层:  eth arp
 */

//protobuf 序列化 程序，json 人可以读的一种 序列化 协议
//Nginx 俄罗斯 大牛 Igor Syseov，服务器web软件
//main 主进程 + work 子进程 工作进程。主进程代码简单就负责子进程的嘎了就重新fork一个。
//子进程就负责复杂的逻辑，http协议处理等等。

int main(void){
	int sv[2];
	int ret = socketpair(AF_UNIX,SOCK_STREAM,0,sv);
	if(ret<0){
		printf("socketpair error,errno: %d\n",errno);
		return -1;
	}
//---- fork() 之前，打开socketpair()
//父子进程之间共享资源：代码，数据、局部变量 / 全局变量 / 内存（写时复制）、文件
	pid_t cpid = fork();
	if(-1 == cpid){
		printf("fork error\n");
		return -1;
	} else if( 0 == cpid){
		close(sv[0]);
		sv[0] = -1;

		char* msg = "hello world !!!";
		size_t len = strlen(msg);
		ret = send(sv[1],msg,len+1,0);
		if(ret < 0){
			printf("send error, errno: %d\n",errno);
			return -1;
		}
		printf("	send ret: %d \n",ret);
		printf("	pid: %d, sended: %s \n",getpid(),msg);
	} else{
		pid_t cpid2 = fork();
		if(-1 == cpid2){
			printf("fork error\n");
			return -1;
		} else if( 0 == cpid2){
			close(sv[1]);
			sv[1] = -1;
			usleep(10*100);
			char buf[1024] = {0};
			ret = recv(sv[0],buf,sizeof(buf)-1,0);
			if(ret < 0){
				printf("recv error,errno: %d\n",errno);
				return -1;
			}
			printf("	recv ret: %d \n",ret);
			printf("	pid: %d, received: %s \n",getpid(),buf);
		}else{
			sleep(2);
		}

	}

	return 0;
}
