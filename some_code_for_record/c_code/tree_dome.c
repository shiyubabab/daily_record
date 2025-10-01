/*************************************************************************
	> File Name: tree_dome.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 30 Sep 2025 04:36:02 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
   二叉树：排序，增 删 改 查，二分查找 O(logN)
   添加元素，添加到 符合排序位置的地方
   二叉树：数据添加上去之后，本身就是 有序的
   定时器：管理结构 用 二叉树
   红黑树：各种数据，比普通二叉树 更平衡
   数据库：B+ 树 机械硬盘，磁道，扇区
   表达式：多叉树
 */

typedef struct tree_s
{
	struct tree_s* left;
	struct tree_s* right; 
	//void*		   data;
	//int		   priority;
	//int          (*handler)(struct tree_s* node);
	int			   d; 
} tree_t;

tree_t* root = NULL;

/*
		 6
		/ \
	    2   7
	   / \ 	 \
	  1  3	 8
		  \
		   5
 */
void tree_insert(tree_t** proot,tree_t* node)
{
	while(*proot){
		if( node->d < (*proot)->d )
			proot = &(*proot)->left;
		else
			proot = &(*proot)->right;
	}
	*proot = node;
}

void tree_recursive_insert(tree_t** proot,tree_t* node)
{
	if(!*proot){
		*proot = node;
		return;
	}

	if(node->d < (*proot)->d)
		tree_recursive_insert(&(*proot)->left,node);
	else
		tree_recursive_insert(&(*proot)->right,node);
}

void tree_del(tree_t** proot, int d)
{
	while(*proot){
		tree_t* r = *proot;
		if(r->d > d)
			proot = &r->left;
		else if(r->d < d)
			proot = &r->right;
		else{
			*proot = r->left;
			if(r->right)
				tree_insert(proot,r->right);
			free(r);
			r=NULL;
			break;
		}
	}
}

void tree_print(tree_t* root){
	if(!root)
		return;
	tree_print(root->left);
	printf("%d\n",root->d);
	tree_print(root->right);
}

int main(void)
{
	root = (tree_t*)malloc(sizeof(tree_t));
	root->d = 5;
	for(int i = 0;i<5;i++){
		tree_t* in = malloc(sizeof(tree_t));
		in -> d = i;
		tree_insert(&root,in);
	}

	for(int i = 10;i>5;i--){
		tree_t* in = malloc(sizeof(tree_t));
		in -> d = i;
		tree_insert(&root,in);
	}
	tree_print(root);

	tree_del(&root,5);

	tree_print(root);

	return 0;
}

