/*************************************************************************
	> File Name: list.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 02 Nov 2025 06:51:51 PM CST
 ************************************************************************/

#include "list.h"

linked_list create_linked_list(void)
{
	linked_list head = (linked_list)malloc(NODESIZE);
	if(head){
		head -> next = head;
		head -> prev = head;
	}
	return head;
}

bool empty_linked_list(linked_list list)
{
	ML_ASSERT(list);
	return list -> next == list || list -> prev == list;
}

size_t size_linked_list(linked_list list)
{
	ML_ASSERT(list);
	int i = 0;
	node_t const * pos = list -> next;
	while(pos != list){
		pos = pos -> next;
		i++;
	}
	return i;
}

void clear_linked_list(linked_list list)
{
	ML_ASSERT(list);
	node_t *pos = list->next,*next;
	for(;pos!=list;pos=next){
		next = pos -> next;
		free(pos);
	}	
	list -> next = list->prev = list;
}

void destroy_linked_list(linked_list list)
{
	ML_ASSERT(list);
	clear_linked_list(list);
	free(list);
}

static node_t * get_node(linked_list list,size_t pos)
{
	node_t * node = list -> next;
	size_t i;
	for(i = 0;i < pos && node != list;i++){
		node = node -> next;
	}

	if(i<pos) return NULL;

	return node;
}

static int insert_before_node(node_t *node,ET elem)
{
	node_t *ins_node = (node_t *)malloc(NODESIZE);
	if(node == NULL){
		return FAILURE;
	}
	ins_node -> elem = elem;
	node->prev->next = ins_node;
	ins_node -> prev = node->prev;
	ins_node -> next = node;
	node -> prev = ins_node;
	return SUCCESS;
}

static int insert_after_node(node_t *node,ET elem)
{
	node_t * ins_node = (node_t *)malloc(NODESIZE);
	if(ins_node == NULL){
		return FAILURE;
	}
	ins_node -> elem = elem;
	node->next->prev = ins_node;
	ins_node -> next = node->next;
	ins_node -> prev = node;
	node -> next = ins_node;
}

int insert_linked_list(linked_list list,size_t pos,ET elem)
{
	ML_ASSERT(list);
	node_t *node = get_node(list,pos);
	if(node == NULL) return FAILURE;
	return insert_before_node(node,elem);
}

int main(void){
	linked_list head = create_linked_list();
	if(empty_linked_list(head)) printf("the linked list is empty !!!\n");
	printf("size of linked list is %ld \n",size_linked_list(head));
	insert_linked_list(head,0,1);
	insert_linked_list(head,1,2);
	if(empty_linked_list(head)) printf("the linked list is empty !!!\n");
	printf("size of linked list is %ld \n",size_linked_list(head));
	return 0;
}

