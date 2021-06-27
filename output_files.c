#include <stdio.h>
#include <stdlib.h>
#include "dataStructures.h"
#include "base64Converter.h"
#include <string.h>

enum {COMMAND, DIRECT, DIRECT_REG}; /* enum to flag the type of the structure that was used for encoding*/ 
/*get the bits that are needed for 64base translation from the bitfield by shifting*/
unsigned int shift_bits(unsigned int x, unsigned int y, int lim)
{	
	int shift = 2, i;
	unsigned int temp;
	for(i = 0; i < lim; i++)
	{
		temp = x&1;
		x = x>>1;
		y = y+(temp<<shift);
		shift ++;
	}
	return y;
}


/*get bits from the instruction table and write them to the output file*/
void print_i_base64(I_node *head, FILE *f)
{
	int state, reg_t, reg_s;
	unsigned int temp = 0;
   	char base64Chars[2];
	I_node *current = head;
	state = COMMAND;
	while (current != NULL)
	{	
		switch(state)
		{
			case COMMAND:	reg_t = current->bits.cmd.r_target; reg_s = current->bits.cmd.r_source;
					temp = ((current->bits.cmd.opcode)&1)<<5;
					temp = current->bits.cmd.ARE + (current->bits.cmd.r_target<<2)+temp;
					base64Chars[1] = convert6BitsToChar(temp); 
					temp = (current->bits.cmd.opcode>>1) + (current->bits.cmd.r_source<<3);
					base64Chars[0] = convert6BitsToChar(temp);  
					fprintf(f,"%c%c\n", base64Chars[0], base64Chars[1]);
					if(reg_s == REGISTER) /*if bits of the source register is direct register*/
						state = DIRECT_REG; /*next bits are in direct_reg struct*/
					else if(reg_s == COMMAND && reg_t != COMMAND)
					{
						if(reg_t != REGISTER)
							state = DIRECT;
						else
							state = DIRECT_REG;
						reg_t = COMMAND;
					}
					else if(reg_s == COMMAND && reg_t == COMMAND)/*if it's stop or rts*/
						state = COMMAND;
					else state = DIRECT; /*otherwise next bits are in direct struct*/
					current = current->next;
					break;
			case DIRECT:
					temp = shift_bits(current->bits.dir.tenBits,current->bits.dir.ARE, 4);
					base64Chars[1] = convert6BitsToChar(temp);
					temp = current->bits.dir.tenBits>>4;
					base64Chars[0] = convert6BitsToChar(temp);
					fprintf(f,"%c%c\n", base64Chars[0], base64Chars[1]);
					if(reg_t == REGISTER)/*if bits of the target register is direct register*/
					{
						reg_t = COMMAND; /*sign that the word after is Command struct*/
						state = DIRECT_REG; /*and next bits are in Direct_reg struct*/
					}
					else if(reg_t == COMMAND)
						{
							state = COMMAND;
							
						}
					else
					{
						reg_t = COMMAND;
						state = DIRECT;
					}
					current = current->next;;
					break;
			case DIRECT_REG:
					temp = shift_bits(current->bits.rdir.dst, current->bits.rdir.ARE, 4);
					base64Chars[1] = convert6BitsToChar(temp);
					temp = (current->bits.rdir.dst>>4) + (current->bits.rdir.src<<1);
					base64Chars[0] = convert6BitsToChar(temp);
					fprintf(f,"%c%c\n", base64Chars[0], base64Chars[1]);
					if(reg_t != REGISTER && reg_t != COMMAND)
					{/*if bits of the target register is direct*/
						reg_t = COMMAND; /*sign that the word after is Command struct*/
						state = DIRECT; /*and next bits are in Direct struct*/
					}
					else 
						{
							state = COMMAND;
							
						}
					current = current->next;
					break;
		}

	}
}


/*get bits from the data table and write them to the output file*/
void print_d_base64(D_node *head, FILE *f)
{
	unsigned int temp;
	binaryCommand* p;
   	char base64Chars[2];
	D_node *current = head;
	while (current != NULL)
	{
		temp = current->par;
      	 	p = (binaryCommand*)&temp;
		getBase64TwoChars(p ,base64Chars);
		fprintf(f,"%c%c\n", base64Chars[0], base64Chars[1]);
		current = current->next;
	}
}


/*write to the object output file*/
void write_obj(FILE *f, int IC, int DC, D_node *d_head,I_node *i_head)
{
	fprintf(f,"%d %d\n", IC-99, DC+1);
	print_i_base64(i_head, f); /*write to the output file the instruction part*/
	print_d_base64(d_head, f); /*write to the output file the data part*/
}


/*write to the extern output file*/
void write_ext(FILE *f, L_node *head)
{
	L_node *current = head;
	while(current != NULL)
	{
		if(current->extOrEnt == EXTERN)
			fprintf(f, "%s	 %d\n", current->label, current->address);
		current = current->next;
	}
}


/*write to the entry output file*/
void write_ent(FILE *f, L_node *head)
{
	L_node *current = head;
	while(current != NULL)
	{
		if(current->extOrEnt == ENTRY)
		{
			fprintf(f, "%s   %d\n", current->label, current->address);
		}
		current = current->next;
	}
}
/*open output files if it's needed for writing in the output*/
void open_files(int IC, int DC, int ENT, int EXT, I_node *i_head, D_node *d_head, L_node *l_head, char *fileName)
{
	char *name = NULL;
	FILE *obj, *ext, *ent;
	name = malloc(strlen(fileName)+6); /* +5 = [.][e][x][t][\0] */ 
	strcpy(name, fileName);
	obj = fopen(strcat(name,".ob"), "wt");
	write_obj(obj, IC, DC, d_head, i_head);
	fclose(obj);
	if(ENT)
	{
		strcpy(name, fileName);
		ent = fopen(strcat(name,".ent"), "wt");
		write_ent(ent, l_head);
		fclose(ent);
	}
	if(EXT)
	{
		strcpy(name, fileName);
		ext = fopen(strcat(name,".ext"), "wt");
		write_ext(ext, l_head);
		fclose(ext);
	}
	free(name);
}
