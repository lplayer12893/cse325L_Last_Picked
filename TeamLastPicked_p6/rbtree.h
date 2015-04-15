#ifndef __RBTREE_H
#define __RBTREE_H

#include <stdio.h>
#include <stdlib.h>

void * getElement(int id);
void insertElement(void * payload, int id);
void deleteElement(int id);

typedef struct _node node;
struct _node
{
	int id;
	node * parent;
	node *left;
	node *right;
	void * payload;
};

#endif // __RBTREE_H