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
 */

typedef struct tree_s
{
	struct tree_s* left;
	struct tree_s* right;
	//void*		   data;
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
		if(node->d < (*proot)->d){
			proot = &(*proot)->left;
		}else{
			proot = &(*proot)->right;
		}
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
			*proot = &r->left;
			if(r->right)
				tree_insert(proot,r->right);
			free(r);
			r = NULL;
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

	return 0;
}

