/*************************************************************************
	> File Name: struct_dome.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 30 Sep 2025 09:40:52 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* 结构体、指针
   1、struct 结构体 关键字，c语言实现 "复杂逻辑" 的基础。
   2、数组 和 结构体 的区别 :
		数组的成员变量		类型一样
		结构体成员变量		类型不一定一样
   3、结构体变量  . 成员变量
   4、结构体指针 -> 成员变量
*/

typedef struct list
{
	struct list* next;	//单链表，"算法容器"
	int				d;
} list_t;

void list_add(list_t** ph,list_t *p)
{
	/*
		函数的形参，局部变量
		1、形参 一级指针， 只是 输入 参数
		2、形参 二级指针， 同时 输入+输出 参数
		3、c语言的函数调用，传值

		p->next = h
		h		= p 修改的 函数的形参，只是修改了"副本"

		4、二级指针 始终指向 要修改的（内存）位置

		c语言的函数调用，传值调用 ==》copy 副本
		形参 是 实参 的 副本 要修改实参 需要 传递 实参的地址
		实参 本身是 指针 的情况下，导致 形参 变成 二级指针
	 */
	p->next = *ph;
	*ph		=  p;
}

void list_del2(list_t** ph,int d)
{
	while(*ph){
		if((*ph)->d == d){
			*ph = (*ph) -> next;
			break;
		}
		ph = &(*ph)->next;
	}
}

int main()
{
	list_t* h = NULL;
	list_t* p = NULL;
	int i;
	for(i=0;i<10;i++){
		p = (list_t *)malloc(sizeof(list_t));
		p->d = i;
		p->next = h;
		h=p;
	}


	list_t* p_n = malloc(sizeof(list_t));
	p_n -> d    = 100;
	list_add(&h,p_n);

	list_del2(&h,0);

	p = h;
	while(p){
		printf("%d\n",p->d);
		p = p->next;
	}


	return 0;
}

