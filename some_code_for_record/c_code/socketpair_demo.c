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
/*
   文件，在用户程序看到的 就是一个 文件句柄
   内核里：
		struct file;

		struct fops{
			int (*open)(const char* path,int flags,int mode);
			int (*write)(struct file* f, const char* __user buf,int count);

			int (*ioctl)(struct file *f, ...);
			int (*mmap)(struct file* f, ...);

			int (*socket)();
			int (*send)();
			int (*recv)();

			int (*sendto)();
			int (*recvfrom)();

			int (*sendmsg)(); scatter/gather
			int (*recvmsg)();

		};

		struct file{
			struct fops* fops;
		};

		struct task_struct{
			struct file** files;
			int			  nb_files;
		};
		文件在两个进程之间的共享：把 内核的 文件的结构体 填写到两个进程的task_struct里

	用户态：
		write(3,"hello",5);
		current->files[3]->fops->write(current -> files[3],buf,count);

	struct msghdr h = {0};
	h.msg_name		= NULL;
	h.msg_namelen   = 0;
	h.msg_flags		= 0;

	struct iovec io = {"f",1};
	h.msg_iov		= &io;
	h.msg_iovlen	= 1;

	struct cmsghdr * cmsg;
	union{
		char buf[CMSG_SPACE(sizeof(int))];
		struct cmsghdr align;
	}u;
//发送 带外数据 控制数据
	h.msg_control	= u.buf;
	h.msg_controllen= sizeof(u.buf);

	cmsg = CMSG_FIRSTHDR(&h);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type  = SCM_RIGHTS;
	cmsg->cmsg_len	 = CMSG_LEN(sizeof(int));
	*(int*)CMSG_DATA(cmsg) = send_fd2;

	ret = sendmsg(sv[1],&h,0);
	if(ret<0){
		printf("error %d\n",errno);
		return -1;
	}

	scatter / gather 拆分/聚合 API
	char* s0 = "hello";
	char* s1 = "world";

	struct iovec v[2] = {{s0,5},{s1,5}};
	h.msg_iov = v;
	h.iovlen  = 2;

	atomic_inc(current->files[5]);
	obj->files[4] = current -> files[5];
*/
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
