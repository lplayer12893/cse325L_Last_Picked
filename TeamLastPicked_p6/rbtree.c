#include "rbtree.h"

void * getElement(node * root, int id)
{
	if (id > root->id)
	{
		return getElement(root->right);
	}
	else if (id < root->id)
	{
		return getElement(root->left);
	}
	else
	{
		return root->payload;
	}
}

void insertElement(node * root, void * payload, int id)
{
	
}

void deleteElement(node * root, int id);
{
	
}
