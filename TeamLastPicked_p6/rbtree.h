#ifndef __RBTREE_H
#define __RBTREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _node node;

void * getElement(node * root, int id);
void insertElement(node * root, void * payload, int id);
void deleteElement(node * root, int id);

#define RED 1
#define BLACK 0


struct _node
{
	int id;
	node * parent;
	node * left;
	node * right;
	void * payload;
	char color;
};

#endif // __RBTREE_H