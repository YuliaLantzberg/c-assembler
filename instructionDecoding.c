#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructures.h"

#define HORIZONTAL_TAB 9
#define NEW_LINE 10
#define SPACE 32
#define COMMA 44
#define ZERO_ASCII 48
#define NINE_ASCII 57
#define BIG_A_ASCII 65
#define BIG_Z_ASCII 90
#define SMALL_A_ASCII 97
#define SMALL_Z_ASCII 122

/*
Build pointers array for an input string.
add pointer for starting and ending substring.
put two pointers (start and end) for comma sign.
The result is pointers array where each part pointed for its start and its end.
*/
void parseStrings(char arrayIn[MAX_SIZE], char* charOut[MAX_SIZE], int* size) {
    int i;
    int status = 0; /* empty - 0, wordChar- 1, comma- 2 */
    for (i = 0; i < MAX_SIZE; i++) {
        if (arrayIn[i] == 0) break;
        if (arrayIn[i] != HORIZONTAL_TAB && arrayIn[i] != NEW_LINE && arrayIn[i] != SPACE) { /* not empty char */
            if (arrayIn[i] == COMMA) { /* comma */
                if (status==1) { /* previous wordChar */
                    charOut[*size] = arrayIn+i-1;
                    *size = *size + 1;
                }
                charOut[*size] = arrayIn+i;
                *size = *size + 1;
                charOut[*size] = arrayIn+i;
                *size = *size + 1;
                status = 2;
            } else { /* wordChar */
                if (status!=1) { /* previous not wordChar */
                    charOut[*size] = arrayIn+i;
                    *size = *size + 1;
                }
                status = 1;
                if (arrayIn[i+1] == 0) { /* last wordChar */
                    charOut[*size] = arrayIn+i;
                    *size = *size + 1;
                }
            }
        } else { /* empty */
            if (status==1) { /* previous wordChar */
                charOut[*size] = arrayIn+i-1;
                *size = *size + 1;
            }
            status = 0;
        }
    }
}

/*
Check if strng contains number int value.
The result is 1 for int valid number,
0- for invalid string.
*/
int isInteger(char* string) {
	int length = 0;
	int i;
	while (string[length] != 0) {
		length++;
	}
	if (!(string[0] == '+' || string[0] == '-' || (string[0] >= ZERO_ASCII && string[0] <= NINE_ASCII))) {
		return 0;
	}
	for (i=1; i < (length-1); i++) {
		if (!(string[i] >= ZERO_ASCII && string[i] <= NINE_ASCII)) {
		    	return 0;
		}
	}
	return atoi(string);
}

/*
The function checks if the given string is a valid label part.
*/
int isValidLabel(char* string) {
    int length = 0;
    int i = 0;
    if (string[0] >= ZERO_ASCII && string[0] <= NINE_ASCII) {
        printf("Error: Not a label- starts with digit. \n");
        return 0;
    }
	while (string[length] != 0) {
		length++;
	}
	if (string[length-1] == ':') {
	    for (i = 0; i < length-1; i++) {
	        if (!((string[i] >= ZERO_ASCII && string[i] <= NINE_ASCII) || (string[i] >= BIG_A_ASCII && string[i] <= BIG_Z_ASCII) || (string[i] >= SMALL_A_ASCII && string[i] <= SMALL_Z_ASCII))) {
    	        return 0;
    	    }
	    }
	    return 1;
	} else {
	    return 0;
	}
}

/*
The function receives an operation part string and decode it to op enum.
*/
int isValidOperation(char* string) {
    if (strcmp(string, "mov") == 0) return MOV;
    if (strcmp(string, "cmp") == 0) return CMP;
    if (strcmp(string, "add") == 0) return ADD;
    if (strcmp(string, "sub") == 0) return SUB;
    if (strcmp(string, "not") == 0) return NOT;
    if (strcmp(string, "clr") == 0) return CLR;
    if (strcmp(string, "lea") == 0) return LEA;
    if (strcmp(string, "inc") == 0) return INC;
    if (strcmp(string, "dec") == 0) return DEC;
    if (strcmp(string, "jmp") == 0) return JMP;
    if (strcmp(string, "bne") == 0) return BNE;
    if (strcmp(string, "red") == 0) return RED;
    if (strcmp(string, "prn") == 0) return PRN;
    if (strcmp(string, "jsr") == 0) return JSR;
    if (strcmp(string, "rts") == 0) return RTS;
    if (strcmp(string, "stop") == 0) return STOP;
    return 0;
}

/*
The function receives an statement part string and decode it to statement enum.
*/
int isValidStatement(char* string) {
    if (strcmp(string, ".data") == 0) return DATA;
    if (strcmp(string, ".string") == 0) return STRING;
    if (strcmp(string, ".entry") == 0) return ENTRY;
    if (strcmp(string, ".extern") == 0) return EXTERN;
    return 0;    
}

/*
The function receives an register name part string and decode it to register index number.
*/
int isValidRegister(char* string) {
    if (strcmp(string, "@r1") == 0) return 1;
    if (strcmp(string, "@r2") == 0) return 2;
    if (strcmp(string, "@r3") == 0) return 3;
    if (strcmp(string, "@r4") == 0) return 4;
    if (strcmp(string, "@r5") == 0) return 5;
    if (strcmp(string, "@r6") == 0) return 6;
    if (strcmp(string, "@r7") == 0) return 7;
    return 0;    
}

/*
The function checks if the given string is a valid label content.
*/
int isValidInputLabelName(char* string) {
    int i = 0;
	if (string[0] != 0 && string[0] >= ZERO_ASCII && string[0] <= NINE_ASCII) return 0;
	while (string[i] != 0) {
	    if (!((string[i] >= ZERO_ASCII && string[i] <= NINE_ASCII) || (string[i] >= BIG_A_ASCII && string[i] <= BIG_Z_ASCII) || (string[i] >= SMALL_A_ASCII && string[i] <= SMALL_Z_ASCII))) {
	        return 0;
	    }
	    if (i > MAX_LABEL_SIZE) { /* label Name max 31 chars */
	        printf("Error: maximal label name length is 31 chars.  \n");
	        return 0;
	    }
		i++;
	}
	/* check if the label is not a RESERVED WORD - (register name/op name/statement name) */
	if (isValidOperation(string) || isValidStatement(string) || isValidRegister(string)) {
		printf("Error: the label name cannot be a reserved word. \n");
		return 0;
	}
    return 1;
}

/*
The function checks if the given string is a valid parameter string part.
*/
int isValidInputString(char* string) {
    int length = 0;
	while (string[length] != 0) {
		length++;
	}
	if (string[0] == '"' && string[length-1] == '"') {
	    return 1;
	}
	printf("Error: Bad string format- check the chars and the quotation marks. \n");
	return 0;
}

/*
The function receives a valid string argumennnnnt and removes the quotes to get the real string content.
*/
void removeStringQuotationMarks(char* string) {
	int length = 0;
	while (string[length] != 0) {
		string[length]=string[length+1];
		length++;
	}
	string[length-2] = 0;
}

/*
Get a substring of an input string, and start and end pointers.
*/
void getStringFromPointers(char arrayOut[MAX_SIZE], char* start, char* end) {
    int i;
    int stringSize = abs(start-end)+1;
    for (i = 0; i < stringSize; i++) {
        arrayOut[i] = *(start+i);
    }
    arrayOut[i] = 0;
}

/*
The function prints the parameter list from the arguments.
*/
void printParametars(parameter* input) {
    parameter* next = input;
    while (next) {
		switch (next->type) {
			case STRING1: printf("String parameter value: %s. \n", (char*)next->value); break;
			case INTEGER: printf("integer parameter value: %d. \n", *((int*)(next->value))); break;
			case LABEL: printf("label parameter value: %s. \n", (char*)next->value); break;
			case REGISTER: printf("register parameter value: @r%d. \n", *((int*)(next->value))); break;
			default: printf("Problem in print arguments."); break;
		}
        next = next->next;
    }
}

/*
The function prints the arguments structure data.
*/
void printArguments(arguments* input) {
    printf("isValid: %d \n isComment: %d \n isLabelExist: %d \n label: %s \n dataType: %d \n opType: %d \n", input->isValid, input->isComment, input->isLabelExist, input->labelData, input->dataType, input->opType);
    printParametars(input->parameters);
}

/*
The function init the arguments structure data.
*/
void initArguments(arguments* args) {
    args->isValid = 0;
    args->isComment = 0;
    args->isLabelExist = 0;
    args->labelData = 0;
    args->dataType = 0;
    args->opType = 0;
    args->parameters = 0;
}

/*
The function checks if the given part is a valid statement or operation.
*/
int operationOrStatementLogic(char outputS[MAX_SIZE], arguments* outputArguments) {
    int operationOrStatement = 0;
    if ((operationOrStatement = isValidOperation(outputS))) {
        outputArguments->opType = operationOrStatement;
    } else if ((operationOrStatement = isValidStatement(outputS))) {
        outputArguments->dataType = operationOrStatement;
    } else {
        printf("Error: fail - not op or statement as expected. \n");
        return 0;
    }
    return 1;
}

/*
The main parsing logic.
The function gets a line and fills the arguments structure.
It puts the operation or statements data with its al parameters in the arguments structure (out).
*/
int parametersLogic(char* splitPointers[MAX_SIZE], arguments* outputArguments, int n) {
	int nextN = 0;
	char parameterString[MAX_SIZE];
	int commaFlag = 1;
	int isFirstParameter = 1;
	parameter* targetOfParam = 0;
	parameter* newParamToWrite = 0;
	int valueIndex = 0;
	
	if (outputArguments->isLabelExist) {
		nextN = 4;
	} else {
		nextN = 2;
	}
	
	/* statement */
	if (outputArguments->dataType == ENTRY || outputArguments->dataType == EXTERN) {
		getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if (isValidInputLabelName(parameterString)) {
			char* labelName = (char*) malloc(MAX_SIZE);
			outputArguments->parameters = malloc(sizeof(parameter));
       			strcpy(labelName ,parameterString);
			outputArguments->parameters->type = LABEL;
			outputArguments->parameters->value = labelName;
			outputArguments->parameters->next = 0;
		} else {
			printf("Error: fail - expected a valid label name. expected a valid label name. \n");
			return 0;
		}
	}
	
	if (outputArguments->dataType == STRING) {
		getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if (isValidInputString(parameterString)) {
			char* stringValue = (char*) malloc(MAX_SIZE);
			removeStringQuotationMarks(parameterString);
			outputArguments->parameters = malloc(sizeof(parameter));
			strcpy(stringValue ,parameterString);
			outputArguments->parameters->type = STRING1;
			outputArguments->parameters->value = stringValue;
			outputArguments->parameters->next = 0;
		} else {
			printf("Error: fail - expected an input string. \n");
			return 0;
		}
	}
	
	if (outputArguments->dataType == DATA) {
		commaFlag = 1;
		while (nextN < n) {
			if (commaFlag) {
			    commaFlag = 0;
				getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
				if (isInteger(parameterString)) {
					int* intValue = (int*) malloc(sizeof(int));
					newParamToWrite = (parameter*) malloc(sizeof(parameter));
					*intValue=atoi(parameterString);
					newParamToWrite->type = INTEGER;
					newParamToWrite->value = intValue;
					newParamToWrite->next = 0;
					if (isFirstParameter) {
						outputArguments->parameters = newParamToWrite;
						targetOfParam = newParamToWrite;
						isFirstParameter = 0;
					} else {
						targetOfParam->next = newParamToWrite;
						targetOfParam = newParamToWrite;
					}
				} else {
					printf("Error: fail - expected an input integer. \n");
					return 0;
				}
			} else {
				getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
				if (strcmp(parameterString, ",") == 0) {
					commaFlag = 1;
				} else {
					printf("Error: fail - expected a comma between parameters. \n");
					return 0;
				}
			}
			nextN = nextN + 2;
		}
		if (commaFlag == 1) {
			printf("Error: bad comma position. should not be a comma at the end of the line. \n");
			return 0;
		}
	}
	
	/* parameters */
	if (outputArguments->opType == NOT ||
    	outputArguments->opType == CLR ||
    	outputArguments->opType == INC ||
    	outputArguments->opType == DEC ||
    	outputArguments->opType == JMP ||
    	outputArguments->opType == BNE ||
    	outputArguments->opType == RED ||
    	outputArguments->opType == JSR
	) {
	    getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if ((valueIndex = isValidRegister(parameterString))) {
			int* regIndex = (int*) malloc(sizeof(int));
			outputArguments->parameters = malloc(sizeof(parameter));
			*regIndex = valueIndex;
			outputArguments->parameters->type = REGISTER;
			outputArguments->parameters->value = regIndex;
			outputArguments->parameters->next = 0;
		} else if (isValidInputLabelName(parameterString)) {
			char* labelName = (char*) malloc(MAX_LABEL_SIZE);
		    outputArguments->parameters = malloc(sizeof(parameter));
			strcpy(labelName ,parameterString);
			outputArguments->parameters->type = LABEL;
			outputArguments->parameters->value = labelName;
			outputArguments->parameters->next = 0;
		} else {
			printf("Error: fail - not a valid parameter for the operation. expected register name or label.  \n");
			return 0;
		}
	}
	
	if (outputArguments->opType == RTS ||
	    outputArguments->opType == STOP
	) {
	    /* no parameters */
	}
	
	if (outputArguments->opType == PRN) {
	    getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if ((valueIndex = isValidRegister(parameterString))) {
			int* regIndex = (int*) malloc(sizeof(int));
			outputArguments->parameters = malloc(sizeof(parameter));
			*regIndex = valueIndex;
			outputArguments->parameters->type = REGISTER;
			outputArguments->parameters->value = regIndex;
			outputArguments->parameters->next = 0;
		} else if (isValidInputLabelName(parameterString)) {
			char* labelName = (char*) malloc(MAX_LABEL_SIZE);
		    outputArguments->parameters = malloc(sizeof(parameter));
			strcpy(labelName ,parameterString);
			outputArguments->parameters->type = LABEL;
			outputArguments->parameters->value = labelName;
			outputArguments->parameters->next = 0;
		} else if (isInteger(parameterString)) {
			int* intValue = (int*) malloc(sizeof(int));
		    outputArguments->parameters = malloc(sizeof(parameter));
			*intValue=atoi(parameterString);
			outputArguments->parameters->type = INTEGER;
			outputArguments->parameters->value = intValue;
			outputArguments->parameters->next = 0;
		} else {
			printf("Error: fail - not a valid parameter for the operation.  \n");
			return 0;
		}
	}
	
	if (outputArguments->opType == LEA) {
		getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if (isValidInputLabelName(parameterString)) {
			char* labelName = (char*) malloc(MAX_LABEL_SIZE);
		    outputArguments->parameters = malloc(sizeof(parameter));
			strcpy(labelName ,parameterString);
			outputArguments->parameters->type = LABEL;
			outputArguments->parameters->value = labelName;
			outputArguments->parameters->next = 0;
			
			nextN = nextN + 2;
			getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
			if (strcmp(parameterString, ",") == 0) {
				nextN = nextN + 2;
				getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
				if ((valueIndex = isValidRegister(parameterString))) {
					int* regIndex = (int*) malloc(sizeof(int));
					outputArguments->parameters->next = malloc(sizeof(parameter));
					*regIndex = valueIndex;
					outputArguments->parameters->next->type = REGISTER;
					outputArguments->parameters->next->value = regIndex;
					outputArguments->parameters->next->next = 0;
				} else if (isValidInputLabelName(parameterString)) {
					char* labelName = (char*) malloc(MAX_LABEL_SIZE);
					outputArguments->parameters->next = malloc(sizeof(parameter));
					strcpy(labelName ,parameterString);
					outputArguments->parameters->next->type = LABEL;
					outputArguments->parameters->next->value = labelName;
					outputArguments->parameters->next->next = 0;
				} else {
					printf("Error: fail - not a valid parameter for the operation. expected register name or label. \n");
					return 0;
				}
			} else {
				printf("Error: fail - expected a comma between parameters. \n");
				return 0;
			}
		} else {
			printf("Error: fail - not a valid label name. \n");
			return 0;
		}
	}
	
	if (outputArguments->opType == CMP) {
		getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if ((valueIndex = isValidRegister(parameterString))) {
			int* regIndex = (int*) malloc(sizeof(int));
			outputArguments->parameters = malloc(sizeof(parameter));
			*regIndex = valueIndex;
			outputArguments->parameters->type = REGISTER;
			outputArguments->parameters->value = regIndex;
			outputArguments->parameters->next = 0;
		} else if (isValidInputLabelName(parameterString)) {
			char* labelName = (char*) malloc(MAX_LABEL_SIZE);
		    outputArguments->parameters = malloc(sizeof(parameter));
			strcpy(labelName ,parameterString);
			outputArguments->parameters->type = LABEL;
			outputArguments->parameters->value = labelName;
			outputArguments->parameters->next = 0;
		} else if (isInteger(parameterString)) {
			int* intValue = (int*) malloc(sizeof(int));
		    outputArguments->parameters = malloc(sizeof(parameter));
			*intValue=atoi(parameterString);
			outputArguments->parameters->type = INTEGER;
			outputArguments->parameters->value = intValue;
			outputArguments->parameters->next = 0;
		} else {
			printf("Error: fail - not a valid parameter for the operation. \n");
			return 0;
		}
		
		nextN = nextN + 2;
		getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if (strcmp(parameterString, ",") == 0) {
			nextN = nextN + 2;
			getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
			if ((valueIndex = isValidRegister(parameterString))) {
				int* regIndex = (int*) malloc(sizeof(int));
				outputArguments->parameters->next = malloc(sizeof(parameter));
				*regIndex = valueIndex;
				outputArguments->parameters->next->type = REGISTER;
				outputArguments->parameters->next->value = regIndex;
				outputArguments->parameters->next->next = 0;
			} else if (isValidInputLabelName(parameterString)) {
				char* labelName = (char*) malloc(MAX_LABEL_SIZE);
				outputArguments->parameters->next = malloc(sizeof(parameter));
				strcpy(labelName ,parameterString);
				outputArguments->parameters->next->type = LABEL;
				outputArguments->parameters->next->value = labelName;
				outputArguments->parameters->next->next = 0;
			} else if (isInteger(parameterString)) {
				int* intValue = (int*) malloc(sizeof(int));
				outputArguments->parameters->next = malloc(sizeof(parameter));
				*intValue=atoi(parameterString);
				outputArguments->parameters->next->type = INTEGER;
				outputArguments->parameters->next->value = intValue;
				outputArguments->parameters->next->next = 0;
			} else {
				printf("Error: fail - not a valid parameter for the operation. \n");
				return 0;
			}
		} else {
			printf("Error: fail - expected a comma between parameters. \n");
			return 0;
		}
	}
	
	if (outputArguments->opType == MOV ||
		outputArguments->opType == ADD ||
	    outputArguments->opType == SUB
	) {
	    getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if ((valueIndex = isValidRegister(parameterString))) {
			int* regIndex = (int*) malloc(sizeof(int));
			outputArguments->parameters = malloc(sizeof(parameter));
			*regIndex = valueIndex;
			outputArguments->parameters->type = REGISTER;
			outputArguments->parameters->value = regIndex;
			outputArguments->parameters->next = 0;
		} else if (isValidInputLabelName(parameterString)) {
			char* labelName = (char*) malloc(MAX_LABEL_SIZE);
		    outputArguments->parameters = malloc(sizeof(parameter));
			strcpy(labelName ,parameterString);
			outputArguments->parameters->type = LABEL;
			outputArguments->parameters->value = labelName;
			outputArguments->parameters->next = 0;
		} else if (isInteger(parameterString)) {
			int* intValue = (int*) malloc(sizeof(int));
		    outputArguments->parameters = malloc(sizeof(parameter));
			*intValue=atoi(parameterString);
			outputArguments->parameters->type = INTEGER;
			outputArguments->parameters->value = intValue;
			outputArguments->parameters->next = 0;
		} else {
			printf("Error: fail - not a valid parameter for the operation. \n");
			return 0;
		}
		
		nextN = nextN + 2;
		getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
		if (strcmp(parameterString, ",") == 0) {
			nextN = nextN + 2;
			getStringFromPointers(parameterString, splitPointers[nextN], splitPointers[nextN+1]);
			if ((valueIndex = isValidRegister(parameterString))) {
				int* regIndex = (int*) malloc(sizeof(int));
				outputArguments->parameters->next = malloc(sizeof(parameter));
				*regIndex = valueIndex;
				outputArguments->parameters->next->type = REGISTER;
				outputArguments->parameters->next->value = regIndex;
				outputArguments->parameters->next->next = 0;
			} else if (isValidInputLabelName(parameterString)) {
				char* labelName = (char*) malloc(MAX_LABEL_SIZE);
				outputArguments->parameters->next = malloc(sizeof(parameter));
				strcpy(labelName ,parameterString);
				outputArguments->parameters->next->type = LABEL;
				outputArguments->parameters->next->value = labelName;
				outputArguments->parameters->next->next = 0;
			} else {
				printf("Error: fail - not a valid parameter for the operation. expected register name or label. \n");
				return 0;
			}
		} else {
			printf("Error: fail - expected a comma between arguments. \n");
			return 0;
		}
	}
	
	if (n-2 > nextN) {
	        printf("Error: fail - too much arguments n:%d nextN:%d \n",n,nextN);
			return 0;
	}
	return 1;
}

/*
Return 1 if line is not empty line and 0 if empty.
*/
int checkNotEmptyLine(char lineData[MAX_SIZE]) {
	int i = 0;
	while (lineData[i] != 0) {
		if (lineData[i] != HORIZONTAL_TAB && lineData[i] != NEW_LINE && lineData[i] != SPACE) {
			return 1;
		}
		i++;
	}
	return 0;
}

 /*
 The main interface for decoding operation / statement line.
 The decoding main function.
 */
void decodeDataFromLine(char commandString[MAX_SIZE], arguments* outputArguments) {
    int n=0;
    int i=0;
	char* splitPointers[MAX_SIZE];
	char outputS[MAX_SIZE];
	
    initArguments(outputArguments);
	outputArguments->isValid = 1;
	
	if ( ! checkNotEmptyLine(commandString)) { /*skip empty line */
		outputArguments->isValid = 0;
		outputArguments->isComment = 1;
		return;
	}

    parseStrings(commandString, splitPointers ,&n);
    if (n >= 2) {
        getStringFromPointers(outputS, splitPointers[0], splitPointers[1]);
		if (outputS[0] == ';') { /* commented line */
			outputArguments->isValid = 0;
			outputArguments->isComment = 1;
			return;
		}
        if (isValidLabel(outputS)) { /* label check */
			char* labelColumn;
            i=0;
            while(outputS[i] != 0) {
                i++;
            }
            outputS[i-1] = 0;
            
            outputArguments->isLabelExist = 1;
            labelColumn = (char*) malloc(MAX_LABEL_SIZE);
            strcpy(labelColumn ,outputS);
            isValidInputLabelName(labelColumn);
            outputArguments->labelData = labelColumn;
            /* now, operation or statement */
            if (n >= 4) {
                getStringFromPointers(outputS, splitPointers[2], splitPointers[3]);
                if (operationOrStatementLogic(outputS, outputArguments) == 0) {
					printf("Error: fail - failed to decode the line data. - bad structure. \n");
					printf("Problem at line: %s \n", commandString);
					outputArguments->isValid = 0;
					return;
				}
				if ( ! parametersLogic(splitPointers, outputArguments, n)) {
					printf("Error: fail - failed to decode the line data. - bad structure. \n");
					outputArguments->isValid = 0;
					return;
				}
            }  else {
				printf("Error: fail - failed to decode the line data. - bad structure. \n");
				printf("Problem at line: %s \n", commandString);
				outputArguments->isValid = 0;
				return;
			}
        } else if (operationOrStatementLogic(outputS, outputArguments) != 0) {
			if( ! parametersLogic(splitPointers, outputArguments, n)) {
				printf("Error: fail - failed to decode the line data. - bad structure. \n");
				printf("Problem at line: %s \n", commandString);
				outputArguments->isValid = 0;
				return;
			}
		} else {
            printf("Error: fail - failed to decode the line data. - bad structure. \n");
			printf("Problem at line: %s \n", commandString);
			outputArguments->isValid = 0;
			return;
        }
    } else {
        printf("Error: fail - failed to decode the line data. - bad structure. \n");
		printf("Problem at line: %s \n", commandString);
		outputArguments->isValid = 0;
        return;
    }
}

/*
int main()
{
    arguments arg1;
    char commandString[MAX_SIZE];
    fgets(commandString, MAX_SIZE, stdin);
    decodeDataFromLine(commandString, &arg1);
    printArguments(&arg1);
     
    return 0;
}
*/

