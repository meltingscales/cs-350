#include <stdio.h>
#include <stdlib.h>	  // For error exit()
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MEMLEN 65536
#define WORDLEN 65535
#define NREG 10
#define DEBUG 1
#define LETTER_DICT "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

typedef long int Word;		 // type that represents a word of SDC memory
typedef unsigned char Address;	 // type that represents an SDC address


void printIf(char* stringy, int booly){if(booly){printf("%s",stringy);}}//debug
int ipow(int a, int b){return ((int)pow(a,b));}

bool strSame(char* strA, char* strB){if(strcmp(strA,strB) == 0){return true;}else{return false;}}

void initIntArr(int* arr, int size){for(int i=0; i<size; i++){arr[i] = 0;}}
void initCharArr(char* arr, int size){for(int i=0; i<size-1; i++){arr[i] = 0;}arr[size]='\0';}

void printIntArr(int* arr, int size){for(int i=0; i<size; i++){printf("[%d] %d",i,arr[i]);}}

void printFree(char* thing){printf("%s",thing);free(thing);}

int strToBin(char* aString)
{ //"0111" -> 7
	char *str = aString; //pass just the value, NOT address
	int ret = 0b0;

	for(int i=0; i<strlen(str); i++)
	{
		if(DEBUG){printf("'%c'\n",(str[i]));}
		if(str[i] == '1')
		{
			if(DEBUG){printf("\nFound a '1' at pos %d.\n",i);
			printf("About to do ret = %d + %d, or %d.\n",(ret),(ipow(2,(strlen(str)-i-1))),(ret+ipow(2,(strlen(str)-i-1))));}

			ret = ret + (ipow(2,(strlen(str)-i-1)));
		}
	}

	printf("%s -> %d\n",aString,ret);
	return ret;
}

char* intToBaseX(unsigned long long value, int base)
{ // 1234 -> 4D2
	unsigned long long converted_len, to_convert;
	int column = 0;

	to_convert = value;

	while(ipow(base,column) <= to_convert)
	{
		column++; //i.e. _16 of 256 has F00 or 3 places...
	}

	int converted_arr[column];
	initIntArr(converted_arr,column);

	for(int i=column; i>=0; i--)
	{
		while(to_convert >= ipow(base,i))
		{
			//printf("About to %d - %d \n",to_convert,((int)(pow(base,i))));

			to_convert += -ipow(base,i); //take away our 1 or 2 or 4 or 8...
			converted_arr[i]++;					 //add on our thing we just took away

			//printf("Added one to %d^%d, left with %d\n",base,i,to_convert);
			//printf("arr[%d]=%d\n",i,converted_arr[i]);
		}
	}

	char *converted_string = malloc(column+1);

	for(int i = column; i>0; i--) //list is backwards
	{
		//printf("At i=%d, should be %c\n",i,LETTER_DICT[converted_arr[i-1]]);
		converted_string[column-i] = LETTER_DICT[converted_arr[i-1]];
	}

	//printf("fml bro %s\n",converted_string);

	converted_string[column] = '\0';

	return converted_string;
}


//Our CPU object
struct CPU
{
	int mem[MEMLEN];
	int reg[NREG];	     // Note: "register" is a reserved word
	Address pc;	     // Program Counter
	int running;	     // running = 1 iff CPU is executing instructions
	Word ir;	     // Instruction Register
	int instr_sign;	     //	  sign of instruction
	int opcode;	     //	  opcode field
	int reg_R;	     //	  register field
	int addr_MM;	     //	  memory field
};

typedef struct CPU CPU;


int main(int argc, char *argv[]);
void initialize_control_unit(CPU *cpu);
void initialize_memory(int argc, char *argv[], CPU *cpu);
FILE *get_datafile(int argc, char *argv[]);

void dump_control_unit(CPU *cpu);
void dump_memory(CPU *cpu);
void print_instr(int instr);
void dump_registers(CPU *cpu);

int read_execute_command(CPU *cpu);
int execute_command(char *cmd_char, CPU *cpu);
void help_message(void);
void many_instruction_cycles(int nbr_cycles, CPU *cpu);
void one_instruction_cycle(CPU *cpu);
//Prototypess



int main(int argc, char *argv[])
{
	CPU cpu_value, *cpu = &cpu_value;

	initialize_memory(argc, argv, cpu);
	printf("printing some vals...\n");

	dump_memory(cpu);


	printf("10 in base 2? %s\n",intToBaseX(10,2));

	printf("26 in base 2? %s\n",intToBaseX(26,2));

	printf("3735928559 aka DEADBEEF in base 16? %s\n",intToBaseX(3735928559,16));

	printf("hi\n");

	return 0;
}

FILE *get_datafile(int argc, char *argv[])
{
	char *default_datafile_name = "default.bin";
	char *datafile_name = NULL;

	if(argv[1] == NULL) //no file location given
	{
		printf("No file specified, using default, or '%s'\n",default_datafile_name);
		datafile_name = default_datafile_name; //set it to default
	}
	else //arg given
	{
		datafile_name = argv[1]; //set to arg
	}

	FILE *datafile = fopen(datafile_name,"r"); //open as read-only

	if(datafile == NULL)
	{
		printf("Opening datafile failed, exiting...\n");
		exit(EXIT_FAILURE);
	}

	return datafile; //return file pointer

}

void initialize_memory(int argc, char *argv[], CPU *cpu)
{
	FILE *datafile = get_datafile(argc, argv);

	printf("initializing memory\n");

	#define DATA_BUFFER_LEN 256
	char buffer[DATA_BUFFER_LEN];

	int value_read, words_read, loc = 0, done = 0;
	char string_read[DATA_BUFFER_LEN];

	char *read_success; //NULL if line read fails
	read_success = fgets(buffer, DATA_BUFFER_LEN, datafile); //if we read in anything

	while(read_success != NULL && !done)
	{
		words_read = sscanf(buffer, "%s", &string_read);
		if(DEBUG){printf("Just read in '%s'\n",string_read);}

	 	value_read = strToBin(string_read);

		//printf("put processing code here\n");

		if(loc <= MEMLEN) //we're within bounds of memory, not too low/high
		{
			if(abs(value_read) <= WORDLEN) //we're a valid hex/dec/binary number
			{
				cpu->mem[loc] = value_read; //read in value
			}
			else
			{
				printf("Sentinel encountered, %d > %d at loc %d\n",value_read,WORDLEN,loc);
				done = true;//sentinel encountered
			}
		}
		else //trying to reference memory that doesn't exist
		{
			printf("Hit beginning/end of memory at loc %d\n",loc);
			done = true;
		}
		read_success = fgets(buffer, DATA_BUFFER_LEN, datafile); //next line pls
		loc++;
	}

	for(int i = loc; i<MEMLEN; i++)
	{
		cpu->mem[i] = 0; //to set it to nothin'
	}
}

void dump_memory(CPU *cpu)
{
	for (int i = 0; i < MEMLEN; i++)
	{
		if(cpu->mem[i] != 0)
		{
			printf("[%4d]  %d\n",i,cpu->mem[i]);
		}
	}
}
