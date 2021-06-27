#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructures.h"
/*macros to create linked list and to add node to list*/
#define CREATE_LIST(TYPE, FUNC_NAME)\
void FUNC_NAME(TYPE **node)\
{\
	TYPE *new_node = (TYPE*)malloc(sizeof(TYPE));\
	if((new_node) == NULL)\
	{\
		printf("out of memory\n");\
		exit(1);\
	}\
	new_node->next = NULL;\
	*node = new_node;\
}

#define ADD_NODE(TYPE, FUNC_NAME, FUNC)\
void FUNC_NAME(TYPE **head, TYPE **tail)\
{\
	TYPE *new_node = NULL;\
	TYPE *cursor = *head;\
	while(cursor->next != NULL)\
		cursor = cursor->next;\
	FUNC(&new_node);\
	cursor->next = new_node;\
	*tail = new_node;\
}

CREATE_LIST(I_node, create_ilist)
ADD_NODE(I_node, add_inode, create_ilist)
CREATE_LIST(D_node, create_dlist)
ADD_NODE(D_node, add_dnode, create_dlist)
CREATE_LIST(L_node, create_llist)
ADD_NODE(L_node, add_lnode, create_llist)
/* this function checks if the label exist in the label table*/
L_node * check_label(L_node *head, char *labl)
{
	L_node *current = head;
	while(current != NULL)
	{
		if(!strcmp(current->label,labl))
		{
			return current;
		}
		current = current -> next;
	}
	return current;
}
