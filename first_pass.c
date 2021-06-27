#include <stdio.h>
#include <stdlib.h>
#include "dataStructures.h"
#include "passUtil.h"
#include "first_pass.h"
#include "instructionDecoding.h"

/*add the additional word for the addressing of the parameters in the command*/
void add_word(I_node **head, I_node **ptr, int *IC, int value, int type, int reg)
{
	*IC = (*IC+1);
	add_inode(head, ptr);/*add the words to hold operands*/
	(*ptr)->I_address = *IC;
	if(type == INTEGER) /*in the case of direct addressing*/
	{	(*ptr)->bits.dir.tenBits = value; 
		(*ptr)->bits.dir.ARE = 0;
	}
	else if (type == REGISTER)/*in the case of direct register addressing*/
	{
		if(reg == 0)
		{
			(*ptr)->bits.rdir.src = value; 
			(*ptr)->bits.rdir.dst = 0; 
		}
		else if(reg == 2)
		{
			(*ptr)->bits.rdir.dst = value;
			(*ptr)->bits.rdir.src = 0;
		}
		else (*ptr)->bits.rdir.dst = value;
		(*ptr)->bits.rdir.ARE = 0;
	}
	
}


/*add the first word for encoding of the command and add additional words according to the type of the addressing*/
void add_first_word(arguments arg, int *IC, I_node **ptr, I_node **head)
{
	(*ptr)->I_address = *IC;
	(*ptr)->bits.cmd.opcode = (arg.opType)-1;
	(*ptr)->bits.cmd.ARE = 0;
	/*encode regarding the command's type*/
	if(arg.opType == RTS || arg.opType == STOP)/*if there no operands*/
	{
		(*ptr)->bits.cmd.r_source = 0; 
		(*ptr)->bits.cmd.r_target = 0;
	}
	else if((arg.opType >= MOV && arg.opType <= SUB)|| arg.opType == LEA)/*if there is the sourse operand*/
	{
		(*ptr)->bits.cmd.r_target = arg.parameters->next->type;
		(*ptr)->bits.cmd.r_source = arg.parameters->type;
		add_word(head, ptr, IC, *(int*)arg.parameters->value, (*ptr)->bits.cmd.r_source, 0);
		if ((arg.parameters->type == INTEGER)||(arg.parameters->type == LABEL)) {				
			add_word(head, ptr, IC, *(int*)arg.parameters->next->value, arg.parameters->next->type, 1);
		}
		else if (arg.parameters->type == REGISTER && arg.parameters->next->type != REGISTER) {
			add_word(head, ptr, IC, *(int*)arg.parameters->next->value, arg.parameters->next->type, 2);
		}
		else 
		{
			(*ptr)->bits.rdir.dst = *(int*)arg.parameters->next->value;
		}
	}
	else /* if there is one operand (destination operand) */
	{
		(*ptr)->bits.cmd.r_source = 0;
 		(*ptr)->bits.cmd.r_target = arg.parameters->type;
		add_word(head, ptr, IC, *(int*)arg.parameters->value, arg.parameters->type, 1);
	}
	
}

/*
function adds words for command .string (the array of chars)
*/
void add_str_word(arguments arg, int *DC, D_node **ptr, D_node **head)
{
	char *str = (char*)(arg.parameters->value);
	(*ptr)->D_address = *DC;
	(*ptr)->par = (*str);
	str = str + 1;
	while(*str!='\0')
	{
		*DC = (*DC+1);
		add_dnode(head, ptr);
		(*ptr)->D_address = *DC;
		(*ptr)->par = (*str);
		str = str + 1;
	} 
	*DC = (*DC+1);
	add_dnode(head, ptr);
	(*ptr)->D_address = *DC;
	(*ptr)->par = 0;
}

/*function adds words for command .data (the array of integers)*/
void add_int_word(arguments arg, int *DC, D_node **ptr, D_node **head)
{
	parameter *current;
	(*ptr)->D_address = *DC;
	(*ptr)->par = *(int*)(arg.parameters->value);
	current = arg.parameters->next;
	while(current != NULL)
	{
		*DC = (*DC+1);
		add_dnode(head, ptr);
		(*ptr)->D_address = *DC;
		(*ptr)->par = *(int*)(current->value);
		current = current->next;
	}
}

/*add label to the label table*/
void add_to_ltable(arguments arg, int addr, L_node *ptr)
{
	ptr->label = arg.labelData;
	ptr->address = addr;
	if(arg.dataType == EXTERN)
	{
		ptr->extOrEnt = EXTERN;
		ptr->label = (char*)(arg.parameters->value);
		ptr->address = 0;
	}
	else if(arg.dataType == 0)
	{
		ptr->action = YES;
		ptr->extOrEnt = 0;
	}
	else  
	{
		ptr->action = NO;
		ptr->extOrEnt = 0;
	}
	
}

/*fill the instructions table, the data table and the label tabel according to the input in the first pass*/
void first_trans(arguments arg, int *IC, int *DC, int *EXT, I_node **i_head, D_node **d_head, L_node **l_head)
{
	int lab_addr; /*the variable holds the address of the label*/
	I_node *i_tail = NULL;
	D_node *d_tail = NULL;
	L_node *l_tail = NULL;
	if(arg.dataType == 0)
	{	
		if(*i_head == NULL) /*if the table is empty*/
		{
			lab_addr = *IC;
			create_ilist(i_head);
			i_tail = *i_head;
			add_first_word(arg, IC, &i_tail, i_head);		
		}
		else
		{
			*IC = (*IC+1);
			lab_addr = *IC;
			add_inode(i_head, &i_tail);
			add_first_word(arg, IC, &i_tail, i_head);
		}		
	}
	else if(arg.dataType == DATA || arg.dataType == STRING)
	{
		if(*d_head == NULL) /*if the table is empty*/
		{
			lab_addr = *DC;
			create_dlist(d_head);
			d_tail = *d_head;
			if(arg.dataType == DATA)
				add_int_word(arg, DC, &d_tail, d_head);
			else	add_str_word(arg, DC, &d_tail, d_head);	
	
		}
		else
		{
			*DC = (*DC+1);
			lab_addr = *DC;
			add_dnode(d_head, &d_tail);
			if(arg.dataType == DATA)
				add_int_word(arg, DC, &d_tail, d_head);
			else	add_str_word(arg, DC, &d_tail, d_head);
		}	
	}
	if((arg.isLabelExist && arg.dataType != ENTRY) || arg.dataType == EXTERN)
	{
		if(arg.dataType == EXTERN)
		{
			*EXT = 1;
		}
		if(*l_head == NULL) /*if the table is empty*/
		{
			create_llist(l_head);
			add_to_ltable(arg, lab_addr, *l_head);
		}
		else
		{
			add_lnode(l_head, &l_tail);
			add_to_ltable(arg, lab_addr, l_tail);
		}	
	}
}

/*read the input line by line and if it is valid and not an empty line ot comment, send it for the first pass*/
int theFirstPass(FILE *f, int *IC, int *DC, int *EXT, I_node **i_head, D_node **d_head, L_node **l_head)
{	
	char commandString[MAX_SIZE];
	arguments arg1;
	int error = 1;
	while(fgets(commandString, MAX_SIZE, f))
	{
		decodeDataFromLine(commandString, &arg1);
		if(arg1.isValid)
			first_trans(arg1, IC, DC, EXT, i_head, d_head, l_head);
		else if(!arg1.isComment)
			error = 0;
	} 
	rewind(f);
	return error;
}    
