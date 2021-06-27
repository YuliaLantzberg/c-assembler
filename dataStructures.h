#define MAX_SIZE 81
#define MAX_LABEL_SIZE 32
#define FREELIST(TYPE, FUNC_NAME)\
void FUNC_NAME(TYPE *head)\
{\
	TYPE *cursor, *tmp;\
	if(head != NULL)\
	{\
		cursor = head;\
		while(cursor != NULL)\
		{\
			tmp = cursor->next;\
			free(cursor);\
			cursor = tmp;\
		}\
	}\
}




typedef enum {STRING1=0,INTEGER=1, LABEL=3, REGISTER=5} parameter_type;
typedef enum {DATA=1, STRING, ENTRY, EXTERN} data_type;
typedef enum {MOV=1, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP} operation_type;
typedef enum {NO, YES} boolean;

typedef struct parameter {
	parameter_type type;
	void* value;
	struct parameter* next;
} parameter;

typedef struct arguments {
	int isValid; /* 0 - not valid, there is problem in the line. 1- valid line */
	int isComment; /* 0 - not comment, 1- commented line */
	int isLabelExist; /* 0 - no labal for this line. 1- there is a valid label. content in the next field. */
	char* labelData;
	data_type dataType;
	operation_type opType;
	parameter* parameters; /* list of command parameters. */
} arguments;

typedef struct {
	unsigned int ARE: 2;
	unsigned int r_target: 3;
	unsigned int opcode: 4;
	unsigned int r_source: 3;
} Command;

typedef struct {
	unsigned int ARE: 2;
	unsigned int dst: 5;
	unsigned int src: 5;
} Direct_reg;

typedef struct {
	unsigned int ARE: 2;
	unsigned int tenBits: 10;
} Direct;

typedef union {
	Command cmd;
	Direct dir;
	Direct_reg rdir;
} I_Encoding;
/*          **********************                        */

typedef struct I_table {
	int I_address;
	I_Encoding bits;
	struct I_table *next;
} I_node;

typedef struct D_table {
	int D_address;
	unsigned int par: 12; 
	struct D_table *next;
} D_node;
/*         ************************                       */

typedef struct Label_table {
	char *label;
	int address;
	data_type extOrEnt;
	boolean action;
	struct Label_table *next;	
} L_node;


