/*
	JERRETT FOWLER
	JF12328
	
	I, Jerrett Fowler, am the sole author of this program.

	MUST COMPILE WITHOUT WARNINGS USING:
	gcc -std=c99 -pedantic -W -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORDLENGTH 32
#define MAXLINELENGTH 32
#define NUMBEROFOPCODES 21
#define HEXSPACE 20

typedef struct node{
	int flag; //flag for second pass processing
	char* LABEL;
	char* OPCODE;
	char* DATA;
	int *SP;
	struct node* next;
} Reg;

Reg* create_structure(char* line_item);
Reg* add_item_to_structure(char* line_item);
size_t stringlen(char *str1, char *str2);
int S2H(char* string); //String to Hex
void remove_char(char* str, char c);
char *rclean(char* str, char throw_away);
char *lclean(char* str, char throw_away);

int main(int argc, char **argv){
	/***[ SEG FAULT DEBUG ]****
		const char *hello = "Hello, is it me you're looking for?";
		printf("%s\n", hello);
		fflush(stdout);
	****[ SEG FAULT DEBUG ]****/

	/***[ PRINTS HEX FOR LINE CHARACTERS ]****
		for(i = 0; i < MAXLINELENGTH; i++){
			printf("%x", line[i]); //Prints hex for each line
		}
		printf("\n");
	****[ PRINTS HEX FOR LINE CHARACTERS ]****/

	/*for(i = 0; i < MAXLINELENGTH; i++){
		printf("%x ", line[i]); //Prints hex for each line
	}*/
	
	FILE *fp;

	if(argv[1] == NULL || (fp = fopen(argv[1], "r")) == NULL){
		printf("You didn't put a filename in argument %d\n", argc);
		return 0;
	}

	if(fp == NULL){
		printf("Error opening file.\n"); //check if NULL and throw error
		return 0;
	}

	char line[MAXLINELENGTH];
	char line_item[MAXWORDLENGTH];
	char buf_label[MAXWORDLENGTH];
	char buf_opcode[MAXWORDLENGTH];
	char buf_data[MAXWORDLENGTH];

	memset(line, 0, sizeof(line[0]) * MAXLINELENGTH);
	memset(line_item, 0, sizeof(line_item[0]) * MAXWORDLENGTH);
	memset(buf_label, 0, sizeof(buf_label[0]) * MAXWORDLENGTH);
	memset(buf_opcode, 0, sizeof(buf_opcode[0]) * MAXWORDLENGTH);
	memset(buf_data, 0, sizeof(buf_data[0]) * MAXWORDLENGTH);

	char MNEMONIC[NUMBEROFOPCODES][MAXWORDLENGTH];
	memset(MNEMONIC, 0, sizeof(MNEMONIC[0][0]) * NUMBEROFOPCODES * MAXWORDLENGTH);
		strcpy(MNEMONIC[0], "data");
		strcpy(MNEMONIC[1], "ldc");
		strcpy(MNEMONIC[2], "adc");
		strcpy(MNEMONIC[3], "ldl");
		strcpy(MNEMONIC[4], "stl");
		strcpy(MNEMONIC[5], "ldnl");
		strcpy(MNEMONIC[6], "stnl");
		strcpy(MNEMONIC[7], "add");
		strcpy(MNEMONIC[8], "sub");
		strcpy(MNEMONIC[9], "shl");
		strcpy(MNEMONIC[10], "shr");
		strcpy(MNEMONIC[11], "adj");
		strcpy(MNEMONIC[12], "a2sp");
		strcpy(MNEMONIC[13], "sp2a");
		strcpy(MNEMONIC[14], "call");
		strcpy(MNEMONIC[15], "return");
		strcpy(MNEMONIC[16], "brz");
		strcpy(MNEMONIC[17], "brlz");
		strcpy(MNEMONIC[18], "br");
		strcpy(MNEMONIC[19], "HALT");
		strcpy(MNEMONIC[20], "SET");

	int *PC; //Program Counter array
	char *pch;

	Reg* ASM = create_structure(line_item);
    ASM = add_item_to_structure(line_item);
	Reg* iterate;

	int counter = 0;
	unsigned int i;

//THIS IS HOW TO MANIPULATE STRUCTURES
/*words* myWords = create_words("Hello");
    myWords = add_word(myWords, "World");

    words* iter;
    for (iter = myWords; NULL != iter; iter = iter->next) {
        printf("%s ", iter->str);
    }
    delete_words(myWords);*/
//END HOW TO MANIPULATE STRUCTURES

/******************************[ START OF PASS 1 ]*************************************/

//PASS 1
	//get first line & while line != EOF do
	while(fgets(line, sizeof(line), fp) != NULL){
		counter++;
		lclean(line, ' ');
		lclean(line, '\t');
		rclean(line, ' ');
		rclean(line, '\t');
		sscanf(line, "%20[;^]s", line);
		sscanf(line, "%[^;]s", line); //strips anything after semicolon
		remove_char(line, ';'); //strips semicolon
		printf("000 Line is %s", line);
		
		//IF LINE IS BLANK, CONTINUE
		if(strcmp(line, "\r\n") == 0 || strcmp(line, "\n") == 0 || line == NULL){
			printf("Empty line!\n");
			continue;
		}
		else{
			pch = strtok(line, " \t");
			//check for comments
			while(pch != NULL){
				char *colon = strchr(line, ':');
				// *****START PROCESSING LINE STRING*****
				if(colon != NULL){
					//iterate through the structure array to search for LABELs used
        			for(iterate = ASM; iterate !=  NULL; iterate = iterate->next){
						if(strcmp(pch, iterate->LABEL) == 0){
							//potential error. Flag for 2nd pass
							printf("Label already found in: %s", iterate->LABEL);
							//insert into array of warnings and errors
						}
        			}
        			//if not found, insert into LABEL spot and set strtok NULL
					sscanf(line, "%[^:]s", buf_label);
					printf("111 Line is %s pch is %s\n", line, pch);
					//INSERT LABEL INTO STRUCTURE
					printf("LABEL: %s\n", buf_label);
					pch = strtok(NULL, " ");
       			}
				//iterate through the char array to search OPCODEs
				if(pch != NULL){
					printf("222 Line is %s pch is %s\n", line, pch);
					for(i = 0; i < MAXLINELENGTH; i++){
						printf("%x", pch[i]); //Prints hex for each line
					}
					printf("\n");
					for(i = 0; i < NUMBEROFOPCODES; i++){ //72657475726e
						//printf("MNEMONIC[%d]: %s\n", i, MNEMONIC[i]);
						if(strcmp(MNEMONIC[i], pch) == 0){ //if found
							//insert into OPCODE spot
							sscanf(pch, "%s", buf_opcode);
							printf("OPCODE: %s\n", buf_opcode);
							pch = strtok(NULL, " ");
						}
					}
				}
				if(pch != NULL){
					printf("333 Line is %s pch is %s\n", line, pch);
					//Check for OPERAND
					//check for label
					for (iterate = ASM; NULL != iterate; iterate = iterate->next){
          	    	  //if found
						if(strcmp(pch, iterate->LABEL) == 0){
							printf("LABEL FOUND FOR OPERAND: %s", pch);//do something
						}
        			}
					if(pch == NULL){
						//if token pointer is NULL, throw error
						printf("Token pointer is NULL, there is no OPERAND: %s", pch);
					}
					else{							
						//insert into OPERAND spot or add to ACCUMULATOR
						sscanf(pch, "%[^\n]s", buf_data);
						printf("OPERAND: %s\n", buf_data);
						pch = strtok(NULL, " ");

					}
				}
			}
		}
		//MAKE STRUCT
		//ADD DATA TO STRUCTUREs
		if(){ //IF LABEL

		}
		if(){ //IF OPCODE

		}
		if(){ //IF DATA

		}
		//SET STRUCT->NEXT TO NULL

		//buf_label = NULL;
		//buf_opcode = NULL;
		//buf_data = NULL;
	}

//end PASS 1

/********************************[ START OF PASS 2 ]********************************/

//PASS 2
//READ FIRST STRUCT CELL
	for(iterate = ASM; iterate !=  NULL; iterate = iterate->next){
		if(strcmp(pch, iterate->LABEL) == 0){
		//potential error. Flag for 2nd pass
			printf("Label already found in: %s", iterate->LABEL);
			//insert into array of warnings and errors
		}
		if(strcmp(pch, iterate->OPCODE) == 0){
		//potential error. Flag for 2nd pass
			printf("Label already found in: %s", iterate->LABEL);
			//insert into array of warnings and errors
		}
		if(strcmp(pch, iterate->DATA) == 0){
		//potential error. Flag for 2nd pass
			printf("Label already found in: %s", iterate->LABEL);
			//insert into array of warnings and errors
		}
	}
	//while PC != EOF do
	while(){

		//OPCODES
		//search MNEMONIC TABLE for OPCODE
		//if found
			//if there is a LABEL in OPERAND field then
				//search LABEL TABLE for OPERAND
				for(iterate = ASM; iterate !=  NULL; iterate = iterate->next){
					//if found then
					//store symbol value as operand address
					if(strcmp(pch, iterate->LABEL) == 0){
						//potential error. Flag for 2nd pass
						//printf("Label found found in: %s", iterate->LABEL);
						//insert into array of warnings and errors
					}
					//else
						//store 0 as operand address
						//set error flag (undefined LABEL)
        		}
			//else
				//store operand address
				//assemble the object code instruction

		//else if OPCODE = 'BYTE' or 'LABEL' then
			//convert constant to object code
				//if object code will not fit into current Text record then
					//write Text record to object program
					//initialize new Text record

				//add object code to Text record

	}
	//write ERRORS and WARNINGS to log file

//end (PASS 2
memset(line, 0, sizeof(line[0]) * MAXLINELENGTH);
memset(line_item, 0, sizeof(line_item[0]) * MAXWORDLENGTH);
memset(buf_label, 0, sizeof(buf_label[0]) * MAXWORDLENGTH);
memset(buf_opcode, 0, sizeof(buf_opcode[0]) * MAXWORDLENGTH);
memset(buf_data, 0, sizeof(buf_data[0]) * MAXWORDLENGTH);
fclose(fp);
free(PC);
}

/******************************[ START OF FUNCTIONS ]*******************************/

//JUST FOR REFERENCE
/*typedef struct node{
	int flag; //flag for second pass processing
	char LABEL[MAXWORDLENGTH];
	char OPCODE[MAXWORDLENGTH];
	char DATA[MAXWORDLENGTH];
	int *SP;
	struct node* next;
} Reg;*/


Reg* create_structure(char* line_item){
	Reg* new_line = malloc(sizeof(line_item));
	if(new_line != NULL){
		new_line->LABEL = line_item;
		new_line->OPCODE = line_item;
		new_line->DATA = line_item;
		new_line->next = NULL;
	}
	return new_line;
}


/*words* create_words(char* word) {
        words* newWords = malloc(sizeof(words));
        if (NULL != newWords){
                newWords->str = word;
                newWords->next = NULL;
        }
        return newWords;
}*/


Reg* add_item_to_structure(char* line_item){
    Reg* new_line = create_structure(line_item);
    if(new_line != NULL){
    	new_line->LABEL = line_item;
		new_line->OPCODE = line_item;
		new_line->DATA = line_item;
		new_line->next = NULL;
    }
	return new_line;
}


/*words* add_word(words* wordList, char* word) {
        words* newWords = create_words(word);
        if (NULL != newWords) {
                newWords->next = wordList;
        }
        return newWords;
}*/

size_t stringlen(char *str1, char *str2){
	size_t size = 0;
	while(*str1 && strchr(str2, *str1++)){
		size++;
	}
	return size;
}

void remove_char(char* str, char c){
    char *pread = str, *pwrite = str;
    while (*pread) {
        *pwrite = *pread++;
        pwrite += (*pwrite != c);
    }
    *pwrite = '\0';
}

char *rclean(char* str, char throw_away){
    char *sp = str + strlen(str);
    while(*--sp == throw_away);
    *(sp + 1) = '\0';
    return str;
}

char *lclean(char* str, char throw_away){
    char *sp = str;
    char *p = sp;
    int trash = 0;
    do{
        if (*sp != throw_away || trash){
            trash = 1;
            *p++ = *sp;
        }
    }
    while (*sp++ != '\0');
    return str;
}

/*int S2H(char *string){
	unsigned char buf[512];
	char res[2048];
	static const char* hex_lookup = "0123456789ABCDEF";
	char *p = res;
	for (int i = 0 ; i != 512 ; i++) {
    	*p++ = hex_lookup[buf[i] >> 4];
    	*p++ = hex_lookup[buf[i] & 0x0F];
    	if ((i+1)%16) {
        	*p++ = ' ';
    	} 
    	else {
        	*p++ = '\r';
        	*p++ = '\n';
    	}
	}
	*p = '\0';
}*/

//Two's Complement
//Invert every bit and add one