/*************************************************************************
	> File Name: byteorder.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 14 Sep 2025 06:45:26 PM CST
 ************************************************************************/

#include <stdio.h>

/***********************************************************************************************
	两台不同字节序的主机之间直接传递时，必然错误解释。解决该问题的方法是：
	发送端总是把要发送的数据转化成大端字节序后再发送，接受方总是知道传送的数据总是大端字节序，
	可以根据自身决定是否对接收到数据进行转化。
	因此大端字节序也称为网络字节序。

	#include <netinet/in.h>
	unsigned long int htonl(unsigned long int hostlong);
	unsigned short int htons(unsigned short int hostshort);
	unsigned long int htohl(unsigned long int hostlong);
	unsigned short int htohs(unsigned short int hostshort);

	长整型函数通常用来转换IP地址;
	短整型函数用来转换端口号。
************************************************************************************************/

void byteorder(){
	union{
		short value;
		char union_bytes[ sizeof(short) ];
	} test;
	test.value = 0x0102;
	if ((test.union_bytes[0] == 1) && (test.union_bytes[1]==2)){
		printf("big endian \n");
	} else if ((test.union_bytes[0] == 2) && (test.union_bytes[1]==1)){
		printf("little endian \n");
	} else {
		printf("unknow ... \n");
	}
}


int main(void){
	byteorder();
	return 0;
}
