#include "rbtree.h"

void * getElement(node * root, int id)
{
	if (id > root->id)
	{
		return getElement(root->right, id);
	}
	else if (id < root->id)
	{
		return getElement(root->left, id);
	}
	else
	{
		return root->payload;
	}
}

void insertElement(node * root, void * payload, int id)
{
	// First, malloc a new node.
	node * new = (node *) malloc(sizeof(node));
	if (new == NULL)
	{
		perror("Cannot malloc new node");
		return;
	}
	
	// Set to safe values.
	new->payload = payload;
	new->id = id;
	new->parent = NULL;
	new->left = NULL;
	new->right = NULL;
	new->color = BLACK;
	
	// Find its spot in the tree.
	node * parent = root;
	while(1)
	{
		if (id > parent->id)
		{
			if (parent->right == NULL)
				break;
			else
				parent = parent->right;
		}
		else
		{
			if (parent->left == NULL)
				break;
			else
				parent = parent->left;
		}
	}
	// At this point, we have to determine if we do any tree rotations.
	
	
}

void deleteElement(node * root, int id)
{
	
}
