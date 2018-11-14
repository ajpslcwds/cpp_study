#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include "common.h"

typedef struct node* position;
typedef struct node* Tree;
struct node
{
	int depth;
	int element;
	position parent;
	position lchild;
	position rchild;
	
	void display()
	{		
		cout<<this->element<<"("<<this->depth<<"),";
	}
};

void plr_sort(Tree tr);
void lpr_sort(Tree tr);
void lrp_sort(Tree tr);
void display(Tree tr);

int getdepth(Tree tr);
int depth_diff(Tree tr);
void insert_into_tree(Tree tr,position np);
Tree insert(Tree tr,int element);
position insert_value(Tree tr,int element);
Tree recover_avl(Tree tr,position np);

void test_AVLTree();

#endif
