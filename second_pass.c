#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructures.h"
#include "passUtil.h"
#include "second_pass.h"
#include "instructionDecoding.h"

/*The function to free the list of parameters*/
FREELIST(parameter, freeParameters)
/*
This function updates the addresses of data labels in the label tabel.
*/
void con_addr(int *IC, L_node *l_head)
{
	L_node *current = l_head;
	while(current != NULL)
	{	
		if(current->extOrEnt != EXTERN && current->action == NO)
			current->address = current->address + *IC+1;
		current = current->next;
	}
}

/*
This function complete the decoding in the instructions table during the second pass.
*/
void add_to_itable(I_node *head, L_node *l_head, int addr, int type, int IC, char *name)
{
	I_node *current = head;
	L_node *labl;
	L_node *l_tail = l_head;
	while(current->I_address != IC)
		current = current->next;
	if(type == EXTERN)
	{	
		current->bits.dir.ARE = 1;
		current->bits.dir.tenBits = 0;		
		labl = check_label(l_head, name);
		if(labl != NULL && labl->address == 0)
		{	
			labl->address = IC;
		}
		else
		{
			while(l_tail->next!=NULL)
				l_tail = l_tail->next;
			add_lnode(&l_head, &l_tail);
			l_tail->label = name;
			l_tail->address = IC;
			l_tail->extOrEnt = EXTERN; 
		}
	} 
	else 
	{
		current->bits.dir.ARE = 2;current->bits.dir.tenBits = addr;
	}
}

/*
The second pass logic.
Go throught the instructions and inserts the special entities in the right places in the 
*/
void second_trans(arguments arg, L_node **head, I_node *i_head, int *IC, int *ENT)
{
	L_node *labl = NULL;
	if(arg.dataType == 0 || arg.dataType == ENTRY)
	{
		if(arg.dataType == ENTRY)
		{
			*ENT = 1;
			labl = check_label(*head, (char*)arg.parameters->value);
			if(labl != NULL)
				labl->extOrEnt = ENTRY;
			else printf("Error: This label is not declared\n");
		}
		else
		{
			*IC = (*IC + 1); 
			if((arg.opType >= MOV && arg.opType <= SUB)|| arg.opType == LEA)
			{
				*IC = (*IC+1);
				if(arg.parameters->type == LABEL)
				{
					labl = check_label(*head, (char*)arg.parameters->value);
					if(labl != NULL) 
					{
						*IC = (*IC+1);
						add_to_itable(i_head, *head, labl->address, labl->extOrEnt, *IC, 									(char*)arg.parameters->next->value);
					} 
					else 
					{
						printf("Error: This label is not declared\n");
					}
				}
				if(arg.parameters->type == INTEGER) 
				{
					*IC = (*IC+1);
				} 
				else if(arg.parameters->next->type == LABEL) 
				{
					*IC = (*IC+1);
					labl = check_label(*head, (char*)arg.parameters->next->value);
					if(labl != NULL) {
						add_to_itable(i_head, *head, labl->address, labl->extOrEnt, 							*IC, (char*)arg.parameters->next->value);
					}
					else printf("Error: This label is not declared\n");
				}
			}
			else if(arg.opType != STOP && arg.opType != RTS)
			{	
				*IC = (*IC+1);
				if(arg.parameters->type == LABEL)
				{
					labl = check_label(*head, (char*)arg.parameters->value);
					if(labl != NULL) 
					{
						add_to_itable(i_head, *head, labl->address, labl->extOrEnt, *IC, 								(char*)arg.parameters->value);
					} 
					else 
					{
						printf("Error: This label is not declared\n");
					}
				}	
			}
		}	
	}		
}

/*
The second pass interface.
Doing the second pass logic on the instructions.
*/
int theSecondPass(FILE *f, int *IC, int *DC, int *ENT, I_node *i_head, D_node *d_head, L_node **l_head)
{
	char commandString[MAX_SIZE];
	arguments arg1;
	con_addr(IC, *l_head);
	*IC = 99;
	while(fgets(commandString, MAX_SIZE, f))
	{	
		decodeDataFromLine(commandString, &arg1); 
		if (!arg1.isComment) 
		{
			second_trans(arg1, l_head, i_head, IC, ENT);
		}
	}
	
	freeParameters(arg1.parameters);
	
	return 1;
}
