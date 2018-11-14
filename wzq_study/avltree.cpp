#include "avltree.h"

void plr_sort(Tree tr)
{
	if (NULL == tr)
	{
		return;
	}
	tr->display();
	plr_sort(tr->lchild);
	plr_sort(tr->rchild);
}
void lpr_sort(Tree tr)
{
	if (NULL == tr)
	{
		return;
	}
	lpr_sort(tr->lchild);
	tr->display();
	lpr_sort(tr->rchild);
}
void lrp_sort(Tree tr)
{
	if (NULL == tr)
	{
		return;
	}
	lrp_sort(tr->lchild);
	lrp_sort(tr->rchild);
	tr->display();
}

void display(Tree tr)
{
	cout << "\nÇ°Ğò£º\n\t";
	plr_sort(tr);
	cout << "\nÖĞĞò£º\n\t";
	lpr_sort(tr);
	cout << "\nºóĞò£º\n\t";
	lrp_sort(tr);
	cout << endl;
}

int getdepth(Tree tr)
{
	if (NULL == tr)
	{
		return 0;
	}
	else
	{
		return tr->depth;
	}
}

void update_depth(Tree tr)
{
	if (NULL == tr)
	{
		return;
	}

	int ldepth = getdepth(tr->lchild);
	int rdepth = getdepth(tr->rchild);
	int maxdepth = ldepth >= rdepth ? ldepth : rdepth;
	tr->depth = maxdepth + 1;
}

int depth_diff(Tree tr)
{
	if (NULL == tr)
	{
		return 0;
	}
	else
	{
		return getdepth(tr->lchild) - getdepth(tr->rchild);
	}
}

void insert_into_tree(Tree tr, position np)
{
	if (NULL == tr)
	{
		return;
	}

	if (tr->element >= np->element)
	{
		if (NULL == tr->lchild)
		{
			np->parent = tr;
			tr->lchild = np;
			if (NULL == tr->rchild)
			{
			}
		}
		else
		{
			insert_into_tree(tr->lchild, np);
		}
	}
	else
	{
		if (NULL == tr->rchild)
		{
			np->parent = tr;
			tr->rchild = np;
		}
		else
		{
			insert_into_tree(tr->rchild, np);
		}
	}
}

position insert_value(Tree tr, int element)
{
	position np =  new node;
	np->element = element;
	np->depth = 1;
	np->parent  = NULL;
	np->lchild  = NULL;
	np->rchild  = NULL;
	insert_into_tree(tr, np);
	return np;
}

Tree insert(Tree tr, int element)
{
	position np = insert_value(tr, element);

	if (NULL == tr)
	{
		tr = np;
	}

	recover_avl(tr, np);

	return tr;
}

Tree recover_avl(Tree tr, position np)
{
	int idiff = 0;
	while (NULL != np)
	{
		update_depth(np);
		idiff = depth_diff(np);
		if (idiff > 1 || idiff < -1)
		{
			if (idiff > 1)
			{

			}

			if (idiff < -1)
			{

			}

			if (np->parent == NULL)
			{
				tr = np;
				break;
			}
		}

		np = np->parent;
	}

	return tr;
}

void test_AVLTree()
{
	Tree  tr = NULL;
	tr = insert(tr, 50);
	tr = insert(tr, 80);
	tr = insert(tr, 70);
	tr = insert(tr, 30);
	tr = insert(tr, 10);
	tr = insert(tr, 20);
	tr = insert(tr, 40);
	tr = insert(tr, 90);
	tr = insert(tr, 60);
	display(tr);
}
