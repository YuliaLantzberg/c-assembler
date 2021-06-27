#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructures.h"
#include "output_files.h"
#include "first_pass.h"
#include "second_pass.h"

FREELIST(I_node, freeIlist)
FREELIST(D_node, freeDlist)
FREELIST(L_node, freeLlist)

int main(int argc, char* argv[])
{
	int i;
	char* strFile = NULL;
   	FILE* f;
	if (argc > 1)
	{		
		for (i =1; i < argc; i++)
		{
		int IC = 100;/*address in the instruction table*/
		int DC = 0; /*address in the data table*/
		int EXT = 0;
		int ENT = 0;
		I_node *i_head = NULL;
		D_node *d_head = NULL;
		L_node *l_head = NULL;
		    strFile = malloc(strlen(argv[i])+5); /* +4 = [.][a][s][\0] */
		    if((strFile) == NULL)
		    {
			printf("out of memory\n");
			exit(1);
		    }
		    strcpy(strFile, argv[i]);
		    strcat(strFile, ".as");
		    f = fopen(strFile, "r");
		    if (!f) {
		        printf("ERROR: bad file name. \n"); 
		        exit(1);
			} 
			/*if the first pass is without errors and the second pass is valid, 
			open and write to the output files*/
		    if (theFirstPass(f, &IC, &DC, &EXT, &i_head, &d_head, &l_head)) 
			{
		    	if(theSecondPass(f, &IC, &DC, &ENT, i_head, d_head, &l_head))
				{	
					fclose(f);
					open_files(IC, DC, ENT, EXT, i_head, d_head, l_head, argv[i]);
				}
			}	
		    freeIlist(i_head);
		    freeDlist(d_head);
		    freeLlist(l_head);
		    free(strFile);
		}
	}
	else {
		printf("No files received as parameters. \n");
	}
	return 0;
}
