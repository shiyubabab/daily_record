/*************************************************************************
	> File Name: list.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 02 Nov 2025 06:22:24 PM CST
 ************************************************************************/

#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define SUCCESS  0
#define FAILURE -1
#define ML_ASSERT(list) assert(list != NULL)

typedef int ET;
typedef struct node_s{
	ET elem;
	struct node_s *prev;
	struct node_s *next;
} node_t;

typedef struct node_s * linked_list; //双向链表

#define NODESIZE sizeof(struct node_s)

linked_list create_linked_list(void);

bool empty_linked_list(linked_list list);

size_t size_linked_list(linked_list list);

void clear_linked_list(linked_list list);

void destroy_linked_list(linked_list list);

int insert_linked_list(linked_list list, size_t pos,ET elem);

int push_front_linked_list(linked_list list, ET elem);

int push_back_linked_list(linked_list list, ET elem);

int remove_position_linked_list(linked_list list,size_t pos);

int pop_front_linked_list(linked_list list);

int pop_back_linked_list(linked_list list);

int remove_elem_linked_list(linked_list list,ET elem);

int remove_elem_all_linked_list(linked_list list,ET elem);

ET *index_of_linked_list(linked_list list, size_t pos);

ET *front_linked_list(linked_list list);

ET *back_linked_list(linked_list list);

int find_elem_linked_list(linked_list list,ET elem);

int count_elem_linked_list(linked_list list,ET elem);

int update_linked_list(linked_list list,size_t pos,ET newElem);

void foreach_linked_list(linked_list list, void (*foreach)(ET));

void reverse_foreach_linked_list(linked_list list,void (*foreach)(ET));

#endif //__LIST_H__
