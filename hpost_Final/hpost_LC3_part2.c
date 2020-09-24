//Henry Post, hpost@hawk.iit.edu, IIT CS350 2016.

#include <stdio.h>
#include <stdlib.h>	  // For error exit()
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MEMLEN 65536
#define WORDLEN 65535
#define NREG 8
#define DEBUG 1
#define LETTER_DICT "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

static const  char* default_datafile_name = "hex/default.hex";


static const char* nzp = "NZP";
static const int opcodeI[] =          {0b0000, 0b0001, 0b0010, 0b0011, 0b0100, 0b0101, 0b0110, 0b0111, 0b1000, 0b1001, 0b1010, 0b1011, 0b1100, 0b1101, 0b1110, 0b1111};
static const char* const opcodeS[] = {"BR   ", "ADD",  "LD",   "ST",   "JSR ", "AND",  "LDR",  "STR",  "RTI",  "NOT",  "LDI",  "STI",  "JMP",  "ERR",  "LEA",  "TRAP"};
static const bool superhappyfuntime = 0;
static const char* superhappystring1 = "\\";
static const char* superhappystring2 = "/";


// CPU Declarations -- a CPU is a structure with fields for the
// different parts of the CPU.
//
typedef          short int Word; // A word of LC-3 memory
typedef unsigned short int Address; // An LC-3 address
typedef               char ConditionCode; // Condition code: <, =, or > 0
typedef unsigned short int Opcode; // Opcode 0-15
typedef               bool Flag; // Boolean flag
typedef unsigned short int Reg; // Register number 0-7

typedef struct
{
  Word mem[MEMLEN];
  Word reg[NREG]; // Note: "register" is a reserved word
  Address pc; // Program Counter
  ConditionCode cc; // positive, zero, or negative
  Flag running; // is CPU executing instructions?
  Word ir; // Instruction Register
  Opcode opcode; // opcode 0-15

} CPU;


void exec_BR(CPU *cpu);   //BR    0000
void exec_ADD(CPU *cpu);  //ADD   0001
void exec_LD(CPU *cpu);   //LD    0010
void exec_ST(CPU *cpu);   //ST    0011
void exec_JSR(CPU *cpu);  //JSR   0100
void exec_AND(CPU *cpu);  //AND   0101
void exec_LDR(CPU *cpu);  //LDR   0110
void exec_STR(CPU *cpu);  //STR   0111
void exec_RTI(CPU *cpu);  //RTI   1000
void exec_NOT(CPU *cpu);  //NOT   1001
void exec_LDI(CPU *cpu);  //LDI   1010
void exec_STI(CPU *cpu);  //STI   1011
void exec_JMP(CPU *cpu);  //JMP   1100
void exec_ERR(CPU *cpu);  //ERR   1101
void exec_LEA(CPU *cpu);  //LEA   1110
void exec_TRAP(CPU *cpu); //TRAP  1111

void printIf(char* stringy, int booly){if(booly){printf("%s",stringy);}}//debug

void replaceChar(char* string, char toReplace, char replacement){for(int i=0; i<strlen(string); i++){if(string[i] == toReplace){string[i] = replacement;}}return;}

int ipow(long int a, int b){int btmp = b;long int ret = 1;while(btmp > 0){ret = ret * a;btmp--;}return ret;}

int logicRShift(int number, int places){return (unsigned)number >> places;}

bool strSame(char* strA, char* strB){if(strcmp(strA,strB) == 0){return true;}else{return false;}}

void initIntArr(int* arr, int size){for(int i=0; i<size; i++){arr[i] = 0;}}
void initCharArr(char* arr, int size){for(int i=0; i<size-1; i++){arr[i] = 0;}arr[size]='\0';}

void fillCharArr(char* arr, int size, char character, char terminal){for(int i=0; i<size-1; i++){arr[i] = character;}arr[size] = terminal;}
void fillIntArr(int* arr, int size, int fillmeupcaptain){for(int i=0; i<size; i++){arr[i] = fillmeupcaptain;}}

void printIntArr(int* arr, int size){for(int i=0; i<size; i++){printf("[%d] %d",i,arr[i]);}}

void printFree(char* thing){printf("%s",thing);free(thing);}

int strcpylen(char* dest, const char* src){strcpy(dest,src);return strlen(src);}

char* intToBaseX(int value, int base);
char* intToOpcode(int opcode);
int charToNum(char character);

char* prettify_mempos(int addr, int val, CPU *cpu);
char* prettify_val_baseX(int val, int base, char padding, int columns);
char* prettify_instruction(int instruction);

char* substring(char *string, int start, int end);
int subint(int integer, int start, int end, int base);

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

void setConditionCode(CPU *cpu, int num);
char cctochar(char cc);
//Prototypess



int main(int argc, char *argv[])
{
	printf("CS350. By Henry Post, hpost@hawk.iit.edu; Final Project part 2.\n");
	CPU cpu_value, *cpu = &cpu_value;

  initialize_control_unit(cpu);
  initialize_memory(argc, argv, cpu);
  dump_control_unit(cpu);
	dump_memory(cpu);

  // Run the command loop
  char *prompt = "> ";
  printf("\nBeginning execution; type h for help.\n");

  int done = 0;

  while (!done)
  {
    printf("%s", prompt);
    done = read_execute_command(cpu);
  }
	return 0;
}

















void dump_memory(CPU *cpu)
{
  printf("   ________________ ________ _____________________ ______________________ \n");
  printf("  |                |        |                     |                      |\n");
  printf("  | xADDR:   xVAL  | DECVAL |       OPCODE        |   ..BINARY...VALUE.. |\n");
  printf("  |________________|________|_____________________|______________________|\n");
  printf("  |                |        |                     |                      |\n");


  int superhappytempval = 0;

	for (int i = 0; i < MEMLEN; i++)
	{

    
    
		if(cpu->mem[i] != 0)
		{
      
      if(superhappytempval == 0)
      {
        superhappytempval = 1;
      }
      else
      {
        superhappytempval = 0;
      }
      char pointer = ' ';

      int unsAddr = twosCompIntToUnsigned(i,16);  //65535
      int unsVal = twosCompIntToUnsigned(cpu->mem[i],16);             //4660

      char* hexAddr = prettify_val_baseX(unsAddr,16,'0',4); // FFFF:
      char* hexVal = prettify_val_baseX(unsVal,16,'0',4);   // x1234

      char charAddr[7] = {0};                               //+4660
      if(cpu->mem[i] < 0)
      {
        sprintf(charAddr,"%d",cpu->mem[i]);
      }
      else
      {
        sprintf(charAddr," %d",cpu->mem[i]);
      }

      char* prettyinstruction = prettify_instruction(cpu->mem[i]);     //ADD -3, -4
      char* binValtmp = prettify_val_baseX(unsVal,2,'0',16);           //1111001110111001
      char* binVal = malloc(16+4+1);                                   //1111 0011 1011 1001

      int tmp = 0;

      for(int i=0; i < 20; i++) //bbbbbbbbbbbbbbbb -> bbbb bbbb bbbb bbbb
      {
        if((i%5)==0)
        {
          binVal[i] = ' ';
        }
        else
        {
          binVal[i] = binValtmp[tmp]; //add from bbbbBbbbbbbbbbbb to bbbb Bbbb bbbb bbbb...increment 1/4 as much...
          tmp++;
        }
      }

      binVal[20] = '\0';




      prettyinstruction[19] = '\0';


      if(cpu->pc == i)
      {
        pointer = '>';
      }
      else
      {
        pointer = '|';
      }
      
      if(!superhappyfuntime)
      {
        printf("  %c x%4s:   x%4s | %6s | %s | %s |\n",pointer,hexAddr,hexVal,charAddr,prettyinstruction,binVal);
      }
      else
      {
        if(superhappytempval)
        {
          printf("  %c x%4s:   x%4s %s %6s %s %s %s %s %s\n",pointer,hexAddr,hexVal,superhappystring2,charAddr,superhappystring2,prettyinstruction,superhappystring2,binVal,superhappystring2);
        }
        else
        {
          printf("  %c x%4s:   x%4s %s %6s %s %s %s %s %s\n",pointer,hexAddr,hexVal,superhappystring1,charAddr,superhappystring1,prettyinstruction,superhappystring1,binVal,superhappystring1);
        }
                
      }
      
      free(hexAddr);
      free(hexVal);
      free(prettyinstruction);
      free(binValtmp);
      free(binVal);

      // printf("cpu->mem[%d] = %d aka %d\n",i,cpu->mem[i],twosCompIntToUnsigned(cpu->mem[i],16));


      // printf("xXx___%s___xXx\n",freeme2); free(freeme2);
		}
	}
  printf("  |________________|________|_____________________|______________________|\n");

}








int read_execute_command(CPU *cpu)
{
// Buffer to read next command line into
#define CMD_LINE_LEN 80
	char cmd_line[CMD_LINE_LEN];
	char *what_we_read_in;		// NULL if reading in a line fails.

	int nbr_cycles;		// Number of instruction cycles to execute
	int is_number = 1;
	char *cmd_char;		// Command 'q', 'h', '?', 'd', or '\n'
	int done = 0;		// Should simulator stop?

	what_we_read_in = fgets(cmd_line, CMD_LINE_LEN, stdin); //fgets returns a *char

	if(what_we_read_in != NULL)
	{
		if(isdigit(*what_we_read_in)) //if first thing is digit
		{
			nbr_cycles = atoi(what_we_read_in); //copy string into int, ignore other shit
			// printf("Doing %d cycles...\n",nbr_cycles);
			many_instruction_cycles(nbr_cycles, cpu);
		}
		else
		{
			done = execute_command(what_we_read_in, cpu);
		}
	}

	return done;
}


void dump_control_unit(CPU *cpu)
{
  printf("CONTROL UNIT:\n");

  char *dcuPC = prettify_val_baseX(cpu->pc,16,'0',4);

  printf("PC = x%s   ",dcuPC);
  free(dcuPC);

  char *dcuIR = prettify_val_baseX(cpu->ir,16,'0',4);

  printf("IR = x%s   ",dcuIR);
  free(dcuIR);

  char dcuCC = '-';

  if(cpu->cc == '<')
  {
    dcuCC = 'N';
  }
  if(cpu->cc == '=')
  {
    dcuCC = 'Z';
  }
  if(cpu->cc == '>')
  {
    dcuCC = 'P';
  }

  printf("CC = %c   ",dcuCC);

  printf("RUNNING: %d\n",cpu->running);


  dump_registers(cpu);
}

void dump_registers(CPU *cpu)
{
  for(int i=0; i<NREG; i++)
  {
    int temp = cpu->reg[i];

    if(cpu->reg[i] < 0)
    {
      temp = (65535 - (~temp));
      // printf("temp was %d, is now %d\n",cpu->reg[i],temp);
    }

    char* drRegval = prettify_val_baseX(temp,16,'0',4);

    if(i == ((int)(NREG / 2))){printf("\n");}

    printf("R%d x%s",i,drRegval);

    printf("  ");

    printf("%6d  ",cpu->reg[i]);

    free(drRegval);
  }
  printf("\n");
}



int execute_command(char *cmd_char, CPU *cpu)
{
	cmd_char[1] = '\0'; //to remove any extra crap...
	// cmd_char = "holy shit i want help"
	// cmd_char = "h\0ly shit i want help"
	//printf("%s",cmd_char) -> "h"

	if(DEBUG)
  {
    printf("we are executing %s\n",cmd_char);
  }

	if((strcmp(cmd_char,"?") == 0) || (strcmp(cmd_char,"h") == 0))
	{
		help_message();
		return 0;
	}

	if(strcmp(cmd_char,"d") == 0)
	{
		dump_control_unit(cpu);
		return 0;
	}

  if(strcmp(cmd_char,"m") == 0)
  {
    dump_memory(cpu);
    return 0;
  }

	if(strcmp(cmd_char,"q") == 0)
	{
		printf("Stopping CPU...\n");
		cpu->running = 0;
		return 1;
	}

	if(strcmp(cmd_char,"\n") == 0)
	{
		one_instruction_cycle(cpu);
		return 0;
	}

	printf("I was given %s, which is not a valid command! Try 'h' for help.\n",cmd_char);
	return 0;
}

void many_instruction_cycles(int nbr_cycles, CPU *cpu)
{
	int INSANE_NBR_CYCLES = 100;

	if(cpu->running > 0)
	{
		if(nbr_cycles > 0 && nbr_cycles <= INSANE_NBR_CYCLES)
		{
			for(int i=0; i<nbr_cycles; i++)
			{
        // if(nbr_cycles > 1){printf("pretending to do %d cycles.\n",nbr_cycles);break;}
				one_instruction_cycle(cpu);
			}
		}
		else
		{
			printf("Was given %d cycles, it's over %d, which is too large. I'll do %d instead.\n",nbr_cycles,INSANE_NBR_CYCLES,INSANE_NBR_CYCLES);
      many_instruction_cycles(INSANE_NBR_CYCLES,cpu);
    }
	}
	else
	{
		printf("CPU is not running, will not execute any instruction cycles!\n");
	}
}


void one_instruction_cycle(CPU *cpu)
{
  // if(cpu->mem[cpu->pc] == 0){return;}
  if(cpu->running == 0)
  {
    printf("Halted\n");
    return;
  }

  int operation = cpu->mem[cpu->pc];
  int opcode = logicRShift((operation & 0xF000),12); //bbbb ---- ---- ----

  char* memaddr16 = prettify_val_baseX(cpu->pc,16,'0',4);
  char* pretty_instruction = prettify_instruction(operation);

  int tmp = operation;

  if(tmp < 0)
  {
    tmp = (65535 - (~tmp));
  }

  char* memval16 = prettify_val_baseX(tmp,16,'0',4);

  pretty_instruction[20] = '\0';

  printf("x%s: x%s  %s; ",memaddr16,memval16,pretty_instruction);

  free(memaddr16);
  free(memval16);
  free(pretty_instruction);

  switch(opcode)
  {

    case 0:
      exec_BR(cpu);
      break;

    case 1:
      exec_ADD(cpu);
      break;

    case 2:
      exec_LD(cpu);
      break;

    case 3:
      exec_ST(cpu);
      break;

    case 4:
      exec_JSR(cpu);
      break;

    case 5:
      exec_AND(cpu);
      break;

    case 6:
      exec_LDR(cpu);
      break;

    case 7:
      exec_STR(cpu);
      break;

    case 8:
      exec_RTI(cpu);
      break;

    case 9:
      exec_NOT(cpu);
      break;

    case 10:
      exec_LDI(cpu);
      break;

    case 11:
      exec_STI(cpu);
      break;

    case 12:
      exec_JMP(cpu);
      break;

    case 13:
      exec_ERR(cpu);
      break;

    case 14:
      exec_LEA(cpu);
      break;

    case 15:
      exec_TRAP(cpu);
      break;
  }
//end of opcode-checking code.

  cpu->pc++; //increment program counter

  printf("\n");


}

void help_message()
{
  printf("    h: help		q: quit		d: dump CPU	 m: dump memory\n    Otherwise, enter a number (or nothing) to execute X CPU cycles.\n");
}


// All the execute_something methods are below.
//

//  0123 4567 89AB CDEF
//
//  bbbb bbbb bbbb bbbb


void exec_BR(CPU *cpu) //BR(NZP), done
{
  int operation = cpu->mem[cpu->pc];
  
  
  int pcoffset9 = (operation & 0x01FF); // ---- ---b bbbb bbbb
  
  int pcoffset9signed = pcoffset9;

  if((operation & 0x0100) > 0) //check if sign is 1 so it's negative
  {
    pcoffset9signed = -(512 + (~pcoffset9signed+1)); //makes it negative
  }



  bool will_branch = true;

  printf("CC = ");

  if(cpu->cc == '<'){printf("N");}
  if(cpu->cc == '='){printf("Z");}
  if(cpu->cc == '>'){printf("P");}

  printf(", ");

       if(((operation & 0x0800) > 0) && (cpu->cc == '<')) // ---- n--- ---- ----
  {
    ;
  }

  else if(((operation & 0x0400) > 0) && (cpu->cc == '='))  // ---- -z-- ---- ----
  {
    ;
  }

  else if(((operation & 0x0200) > 0) && (cpu->cc == '>'))  // ---- --p- ---- ----
  {
    ;
  }

  else
  {
    will_branch = false;
    printf("no branch");
  }

  if(will_branch)
  {




    char* PCA = prettify_val_baseX(cpu->pc+1,16,'0',4);
    char* PCB = prettify_val_baseX(cpu->pc+pcoffset9signed+1,16,'0',4);

    char sign = '+';
    if(pcoffset9signed<0){sign = ' ';}

    printf("PC <- x%s%c%d = x%s",PCA,sign,pcoffset9signed,PCB);

    cpu->pc += pcoffset9signed;

    free(PCA);
    free(PCB);
  }

}

void exec_ADD(CPU *cpu) //ADD, done
{
  int operation = cpu->mem[cpu->pc];
  int imm5 = (operation & 0x001F); //---- ---- ---b bbbb
  if((operation & 0x0010) > 0)
  {
    imm5 = ~imm5 + 1;
  }
  int imm5signed = imm5;

  if(imm5signed < 0)
  {
    imm5signed = -(32 + (~imm5signed+1));
    // printf("\n\nimm5signed is %d, imm5 is %d\n\n",imm5signed,imm5);
  }

  int DR =  (operation & 0x0E00) >> 9; //---- bbb- ---- ----
  int SR1 = (operation & 0x01C0) >> 6; //---- ---b bb-- ----
  int SR2 = (operation & 0x0007) >> 0; //---- ---- ---- -bbb

  // printf("DR = %d, SR1 = %d, SR2 = %d\n",DR,SR1,SR2);
  // printf("cpu->mem[SR1] = %d\n",cpu->reg[SR1]);
  // printf("cpu->mem[SR2] = %d\n",cpu->reg[SR2]);

  int srval = 0;
  int srval2 = 0;

  printf("R%d <- ",DR);

  if((operation & 0x0020) > 0) //if using imm5
  {

    srval = cpu->reg[SR1];


    if(srval < 0)
    {
      srval = (65535 - (~srval));
    }

    char* srvalc = prettify_val_baseX(srval, 16, '0', 4);


    printf("x%s",srvalc);

    if(imm5 >= 0)
    {
      printf(" + %d",imm5);
    }
    else
    {
      printf(" - %d",-imm5);
    }
    srval2 = cpu->reg[SR1] + imm5;

    if(srval2 < 0)
    {
      srval2 = (65535 - (~srval2));
    }

    char* srvalc2 = prettify_val_baseX(srval2, 16, '0', 4);

    printf(" = x%s",srvalc2);

    free(srvalc);
    free(srvalc2);

    cpu->reg[DR] = cpu->reg[SR1] + imm5;
  }
  else //not using imm5
  {
    srval = cpu->reg[SR1];

    if(srval < 0)
    {
      srval = (65535 - (~srval));
    }

    srval2 = cpu->reg[SR2];

    if(srval2 < 0)
    {
      srval2 = (65535 - (~srval2));
    }

    int result = cpu->reg[SR1] + cpu->reg[SR2];

    if(result < 0)
    {
      result = (65535 - (~result));
    }

    // printf("srval = %d, srval2 = %d, result = %d\n",srval,srval2,result);
    // printf("cpu->mem[SR1] = %d, cpu->mem[SR2] = %d, those two added = %d\n",cpu->mem[SR1],cpu->mem[SR2],cpu->mem[SR1]+cpu->mem[SR2]);

    char* SRvalc = prettify_val_baseX(srval,16,'0',4);
    char* SRval2c = prettify_val_baseX(srval2,16,'0',4);

    char* resultc = prettify_val_baseX(result,16,'0',4);
    printf("x%s + x%s = x%s",SRvalc,SRval2c,resultc);

    cpu->reg[DR] = cpu->reg[SR1] + cpu->reg[SR2];


    free(SRvalc);
    free(SRval2c);
    free(resultc);
  }
    setConditionCode(cpu,cpu->reg[DR]);

    printf(" CC = %c",cctochar(cpu->cc));
}

void exec_LD(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

  int DR = (operation & 0x0E00) >> 9; //---- bbb- ---- ----
  int pcoffset9 = operation & 0x01FF; //---- ---b bbbb bbbb
  int pcoffset9signed = pcoffset9;

  if(pcoffset9signed < 0)
  {
    pcoffset9signed = -(512 + (~pcoffset9signed+1));
  }

  char* pcoffset9hex = prettify_val_baseX((pcoffset9signed+cpu->pc+1),16,'0',4);
  char* memvalhex = prettify_val_baseX(cpu->mem[cpu->pc+1+pcoffset9],16,'0',4);

  printf("R%d <- M[x%s] = x%s; ",DR,pcoffset9hex,memvalhex);

  cpu->reg[DR] = cpu->mem[cpu->pc+1+pcoffset9];

  setConditionCode(cpu,cpu->reg[DR]);

  printf("CC = %c",cctochar(cpu->cc));


}

void exec_ST(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

  int SR = (operation & 0x0E00) >> 9;

  int pcoffset9 = (operation & 0x01FF);
  int pcoffset9signed = pcoffset9;

  if((operation & 0x0100) > 0)
  {
    pcoffset9signed = -(512 + (~pcoffset9signed+1));
  }


  int temp = cpu->reg[SR];

  if(cpu->reg[SR] < 0)
  {
    temp = (65535 - (~temp));
    // printf("cpu->reg[SR] = %d, will be %d\n",cpu->reg[SR],temp);
  }

  char* srval = intToBaseX(temp,16);

  char* memloc = intToBaseX(cpu->pc+pcoffset9signed+1,16);

  printf("M[x%s] <- x%s\n",memloc,srval);

  cpu->mem[cpu->pc+pcoffset9signed+1] = cpu->reg[SR];

  free(srval); //(65535 - (~temp))
  free(memloc);
}

void exec_JSR(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

  int pcoffset11 = (operation & 0x01FF);
  int pcoffset11signed = pcoffset11;

  if((operation & 0x100) > 0)
  {
    pcoffset11signed = -(2048 + (~pcoffset11signed+1));
  }

  if((operation & 0x0800) > 0)
  {//we have JSR, using pcoffset11


    char* r7gets = intToBaseX(cpu->pc+1,16);

    char* wherewejump = intToBaseX(cpu->pc+1+pcoffset11signed,16);

    char sign = '+';
    if(pcoffset11signed<0){sign=' ';}

    printf("PC <- x%s %c%d = x%s, R7 <- x%s",r7gets,sign,pcoffset11signed,wherewejump,r7gets);


    cpu->reg[7] = cpu->pc+1;
    cpu->pc += pcoffset11signed;



    free(r7gets);
    free(wherewejump);
  }
  else
  {//we have JSSR, using baseR
    int baseR = (operation & 0x01C0) >> 6;

    int temp = cpu->reg[baseR];
    if(temp < 0)
    {
      temp = (65535 - (~temp));
    }

    char* wherewejump2 = intToBaseX(cpu->pc+cpu->mem[baseR]+1,16);

    char* pcgets = intToBaseX(temp+1,16);

    printf("PC <- x%s, R7 <- x%s",pcgets,wherewejump2);

    cpu->reg[7] = cpu->pc+cpu->mem[baseR]+1;

    cpu->pc = cpu->reg[baseR];


    free(wherewejump2);
    free(pcgets);
  }

  return;
}

void exec_AND(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];


  int DR =   (operation & 0x0E00) >> 9; // ---- bbb- ---- ----
  int SR1 =  (operation & 0x01C0) >> 5; // ---- ---b bb-- ----

  int imm5 = (operation & 0x001F);      // ---- ---- ---b bbbb
  int imm5ext = imm5;
  int imm5extpos = imm5ext;

  int res, resPos = 0;

  if((operation & 0x0010) > 0)
  {
    imm5ext = -((~imm5ext)+33);

    // printf("\n\nimm5ext = %d\n",imm5ext);
    imm5extpos = (65535 + (imm5ext));

    // printf("\nimm5extpos = %d\n",imm5extpos);

    char* test = prettify_val_baseX(imm5extpos+1,16,'0',4);
    // printf("imm5extpos is also %s as a string\n",test);
    free(test);
  }



  int sr1val = cpu->reg[SR1];

  if(sr1val < 0)
  {
    sr1val = (65535 - (~sr1val));
  }





  if((operation & 0x0020) > 0)
  {//using imm5

    char* sr1valc = prettify_val_baseX(sr1val,16,'0',4);

    printf("R%d <- x%s & ",DR,sr1valc);

    free(sr1valc);

    char* imm5c = prettify_val_baseX(imm5extpos+1,16,'0',4);

    printf("x%s = ",imm5c);
    free(imm5c);

    res = (imm5ext & cpu->reg[SR1]);
    resPos = res;

    if(res < 0)
    {
      resPos = (65535 - (~resPos));
    }

    char* resc = prettify_val_baseX(resPos,16,'0',4);

    printf("x%s",resc);
    free(resc);

    cpu->reg[DR] = res;
  }
  else
  {//using SR2

  }


  setConditionCode(cpu,cpu->reg[DR]);
  printf("; CC = %c",cctochar(cpu->cc));

}

void exec_LDR(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

}

void exec_STR(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

}

void exec_RTI(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

}

void exec_NOT(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];


  int DR = (operation & 0x0E00) >> 9; //---- bbb- ---- ----
  int SR = (operation & 0x01C0) >> 6; //---- ---b bb-- ----

  int SRval = cpu->reg[SR];

  if(SRval < 0)
  {
    SRval = (65535 - (~SRval));
  }

  int notSRval = 65535 - SRval; //the offset for pretty-printing

  char* SRvalc = prettify_val_baseX(SRval,16,'0',4);

  char* notSRvalc = prettify_val_baseX(notSRval,16,'0',4);

  printf("R%d <- NOT x%s = x%s;",DR,SRvalc,notSRvalc);

  cpu->reg[DR] = ~(cpu->reg[SR]);

  setConditionCode(cpu,cpu->reg[DR]);

  printf(" CC = %c",cctochar(cpu->cc));

}

void exec_LDI(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

}

void exec_STI(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

}

void exec_JMP(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

  int baseR = (operation & 0x01C0) >> 6;

  int regVal = cpu->reg[baseR];

  int regValuns = regVal;

  if(regValuns < 0)
  {
    regValuns = (65535 - (~regValuns));
  }


  char* regValc = intToBaseX(regValuns,16);

  printf("PC <- x%s",regValc);

  cpu->pc = regVal-1;
}

void exec_ERR(CPU *cpu)
{
  printf("Reserved opcode; ignored");
}

void exec_LEA(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

  int DR = (operation & 0x0E00) >> 9;

  int pcoffset9 = (operation & 0x01FF);
  int pcoffset9signed = pcoffset9;


  if((operation & 0x0100) > 0)
  {
    pcoffset9signed = -(512 + (~pcoffset9signed+1));
  }



  char* drvals = intToBaseX(cpu->pc+pcoffset9signed+1,16);

  cpu->reg[DR] = cpu->pc+pcoffset9signed+1;

  setConditionCode(cpu,cpu->reg[DR]);


  printf("R%d <- x%s; CC = %c",DR,drvals,cctochar(cpu->cc));




}

void exec_TRAP(CPU *cpu)
{
  int operation = cpu->mem[cpu->pc];

  int trapvect8 = (operation & 0x00FF);

  printf("CC = %c; ",cctochar(cpu->cc));

  switch (trapvect8)
  {
    case 0x20:; //GETC
      printf("GETC: ");

      char buf[80];
      initCharArr(buf,80); //to avoid random crap

      char *what_we_read_in;
      char singlechar;

      what_we_read_in = fgets(buf,80,stdin);

      singlechar = what_we_read_in[0];

      printf("Read %c = %d",singlechar,singlechar);

      cpu->reg[0] = singlechar;


      break;

    case 0x21:; //OUT

      int tmp = cpu->reg[0] & 0x00FF;

      printf("OUT: %d = %c",tmp,tmp);
      break;

    case 0x22:; //PUTS

      int stringPos = cpu->reg[0];


      if(stringPos < 0)
      {
        stringPos = (65535 - (~stringPos)); //still dont know. works though! lolololol.
      }

      printf("PUTS: ");

      while(cpu->mem[stringPos] != 0x0000)
      {
        printf("%c",(cpu->mem[stringPos] & 0x00FF));
        stringPos++;
      }

      break;

    case 0x23: //IN

      printf("IN: Input a character>");

      char buf1[80];
      initCharArr(buf1,80); //to avoid random crap. fills w/ zer0es

      char *what_we_read_in1;
      char singlechar1;

      what_we_read_in1 = fgets(buf1,80,stdin);

      singlechar1 = what_we_read_in1[0];

      printf("Read %c = %d\n",singlechar1,singlechar1);

      cpu->reg[0] = singlechar1;

      break;

    case 0x25: //HALT
      printf("Halting");
      cpu->running = 0;
      break;
  }

}
//}

//End of execute_something methods.










/*
;
; some text
x1000
x1234








words_read = sscanf("x9999??bologna; this is a comment","%x??%x",&value_that_we_got,&value_2);

value_that_we_got = 0x9999;
value_2 = ???;

words_read = 1;






words_read = sscanf("bologna ; this is a comment","%x",&value_that_we_got);

value_that_we_got = ???;
words_read = 0;

*/



















char* stringSpacing(char* string, int *spaceLocs, char* spacingChars)
{
  char* ret = malloc(strlen(string) + spaceLocs[0]); //sizeof str + arr[0] assuming we store arrsize in it...
  initCharArr(ret,strlen(string)+spaceLocs[0]);

  printf("\"%s\".len = %d, retsize = %d.\n",string,strlen(string),(strlen(string)+spaceLocs[0]));

  return ret;
}




void initialize_memory(int argc, char *argv[], CPU *cpu)
{
	FILE *datafile = get_datafile(argc, argv);

	#define DATA_BUFFER_LEN 256
	char buffer[DATA_BUFFER_LEN];

	int value_read, words_read, loc = 0, done = 0;
	char string_read[DATA_BUFFER_LEN];
  bool set_start_pos = false;
  int start_pos = 0;

	char *read_success; //NULL if line read fails
	read_success = fgets(buffer, DATA_BUFFER_LEN, datafile); //if we read in anything


  
  int *val1 = 0;
  int *val2 = 0;
  
  
  // int how_many = sscanf("x1234 xABCD","%x",&val1);
  
  // printf("val1 = %4x, val2 = %4x, words_read = %d\n",val1,val2,how_many);
  

	while(read_success != NULL && !done)
	{
    
    // printf("buffer is \" %s \"\n",buffer);
		words_read = sscanf(buffer, "%s", &string_read);
    

    
    
    
    
    
    
		if(DEBUG)
    {
      printf("Just read in '%s'\n",string_read);
    }

	 	value_read = strToBaseX(string_read,16);

    //printf("%s is %d\n",string_read, value_read);


    if(string_read[0] != ';') //ignore semicolons
    {
      if(loc == 0 && !set_start_pos)
      {
        loc = value_read; //to specify start of file
        start_pos = loc;


        char* initmemOrigin = prettify_val_baseX(loc,16,'0',4);
        printf("Origin = x%s\n", initmemOrigin);
        free(initmemOrigin);

        cpu->pc = start_pos;
        set_start_pos = true;
        // printf("starting at %d from our value\n",loc);
      }

  		if(loc <= MEMLEN) //we're within bounds of memory, not too low/high
  		{
        if(loc == start_pos) //don't read it in, it's to specify our starting place
        {
          // printf("not reading in our value because loc=%d=start_pos=%d\n",loc,start_pos);

          read_success = fgets(buffer, DATA_BUFFER_LEN, datafile); //next line pls
          words_read = sscanf(buffer, "%s", &string_read);
      	 	value_read = strToBaseX(string_read,16);

          cpu->mem[loc] = value_read;
        }
  	    else if(abs(value_read) <= WORDLEN) //we're a valid hex/dec/binary number
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
  		loc++;
    }
    else
    {
      // printf("found comment, ignoring...\n");
    }


    read_success = fgets(buffer, DATA_BUFFER_LEN, datafile); //next line pls
	}

	for(int i = loc; i<MEMLEN; i++)
	{
		cpu->mem[i] = 0; //to set it to nothin'
	}
}

FILE *get_datafile(int argc, char *argv[])
{
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


  printf("Loading %s\n",datafile_name);
	return datafile; //return file pointer

}

void initialize_control_unit(CPU *cpu)
{
	cpu->pc = 00; //program counter
	cpu->ir = 0000;
  cpu->running = 1;
  cpu->cc = '=';

  for(int i=0; i < NREG; i++)
  {
    cpu->reg[i] = 0;
  }
}
















char* intToOpcode(int opcode)
{
  return opcodeS[opcode];
}

char* substring(char* string, int start, int end) //USE FREE AFTER...
{
  int size = (end - start + 1);
  char* ret = malloc(size); //"012345"[0:1] gives a 2-long string...

  initCharArr(ret, size);

  int arrPos = 0;
  for(int i=0; i<=end; i++)
  {
    if(i >= start)
    {
      ret[arrPos] = string[i];
      arrPos++;
    }
  }

  return ret;
}

int subint(int integer, int start, int end, int base)
{
  char* integerS = intToBaseX(integer,base); //1000101 -> "1011100"

  char* splicedIntegerS = substring(integerS,start,end); //"1011100"[2:4] -> "111"

  int ret = strToBaseX(splicedIntegerS,base); // "111" -> 111

  free(integerS);
  free(splicedIntegerS);

  return ret;
}


char* prettify_instruction(int signedInstruction)
{
  int instruction = twosCompIntToUnsigned(signedInstruction,16);
  int bufsize = 50;
  char* ret = malloc(bufsize);
  fillCharArr(ret,bufsize,' ',' ');

  int stridx = 0;
  int tmp = 0;

  char* instructionS = prettify_val_baseX(instruction, 2, '0', 16); //0000 0100 0010 1111 -> "0000010000101111"
  char* opcodeS = substring(instructionS,0,3); //0011 -> "0011"

  int opcode = strToBaseX(opcodeS,2); //fixes the 00001 == 1 bug. uses "00001"[0:3] == "0000" == 0...

  char* opcodeS2 = intToOpcode(opcode); //0000 -> "BR???"

  // printf("\n\nprettify_instruction(%d)\n",instruction);
  // printf("%s\n%s",opcodeS,instructionS);
  // printf("about to switch on %d\n",opcode);



  stridx += strcpylen((ret+stridx),opcodeS2);    //adds "ST" to opcode, increments stridx.
  ret[stridx] = ' '; //stops that '\0' from ruining your day

  stridx += ( 6 - strlen(opcodeS2) );  //adds padding, at least 6 spaces.


  //
  //

////////////////////////////////////////  //  0123 4567 89AB CDEF

  int b46 = (instruction & 0x0E00) >> 9;  //  ---- ###- ---- ---- aka DR/SR/nzp

  int b79 = (instruction & 0x01C0) >> 6;  //  ---- ---# ##-- ---- aka SR1/SR/BaseR
  int bDF = (instruction & 0x0007);       //  ---- ---- ---- -### aka SR2

  int bBF = (instruction & 0x001F);       //  ---- ---- ---# #### aka imm5
  int bB  = (instruction & 0x0010) >> 4;  //  ---- ---- ---# ---- aka if imm5 is negative

  int bAF = (instruction & 0x003F);       //  ---- ---- --## #### aka offset6
//int bA  = (instruction & 0x0020) >> 5;  //  ---- ---- --#- ---- aka if offset6 is negative


  int b8F = (instruction & 0x00FF);       //  ---- ---- #### #### aka trapvect8
  int b8  = (instruction & 0x0080) >> 7;  //  ---- ---- #--- ---- aka if trapvect8 is negative

  int b7F = (instruction & 0x01FF);       //  ---- ---# #### #### aka PCoffset9
  int b7  = (instruction & 0x0100) >> 8;  //  ---- ---# ---- ---- aka if PCoffset9 is negative


  int b5F = (instruction & 0x07FF);       //  ---- -### #### #### aka PCoffset11
  int b5  = (instruction & 0x0400) >> 10; //  ---- -#-- ---- ---- aka if PCoffset11 is negative

  int b4  = (instruction & 0x0800) >> 11; //  ---- #--- ---- ---- aka flag bit for JSR/JSRR
  int bA  = (instruction & 0x0020) >> 5;  //  ---- ---- --#- ---- aka flag bit for ADD 'n' AND

  int imm5 = bBF;
  int imm5signed = bBF;

  if(bB == 1)
  {
    imm5signed = -(32 + (~imm5signed+1));
  }


  int offset6 = bAF;
  int offset6signed = bAF;

  if(bA == 1)
  {
    offset6signed = -(64 + (~offset6signed+1));
  }


  int pcoffset9 = b7F;
  int pcoffset9signed = b7F;

  if(b7 == 1)
  {
    pcoffset9signed = -(512 + (~pcoffset9signed+1));
  }


  int pcoffset11 = b5F;
  int pcoffset11signed = b5F;

  if(b5 == 1)
  {
    pcoffset11signed = -(2048 + (~pcoffset11signed+1));
  }


  switch(opcode)
  {
    case 0: //0b000 AKA BRXXX or if NZP == 000, it's data. also, done.

      tmp = stridx; //we're going to fix "BR___". gotta jump around.


      char* nzpBits = substring(instructionS,4,6); //bbbb XXXb bbbb bbbb
      stridx = 2;


      if(strcmp(nzpBits,"000") == 0) //if 000, it's data.
      {//we have data...
        stridx = 0; //overwrite "BR   "...
        stridx += strcpylen(ret + stridx,"NOP   ");
        ret[stridx] = ' '; //for strcpy's '\0'

        if(opcode < 0)
        {
          ret[stridx] = '-';
          stridx++;
        }

        char* brData = intToBaseX(signedInstruction,10);

        stridx += strcpylen(ret+stridx,brData);
        ret[stridx] = ' ';


        stridx += 5 - strlen(brData); //add padding, hopefully
        free(brData);


        ret[stridx] = '\'';
        stridx++;

        if(instruction == '\n')
        {
          stridx += strcpylen(ret+stridx,"\\n"); //avoid teh uglies...
          ret[stridx] = ' ';
        }
        else
        {
          ret[stridx] = instruction & 0x00FF; //print char. COOL HUH???? WOOO!
          stridx++;
        }



        ret[stridx] = '\'';
        stridx++;



        break; //skip other checks and avoid setting pcoffset9
      }

      else if(strcmp(nzpBits, "111") == 0) //BR is easier than BRNZP.
      {
        stridx += strcpylen((ret + stridx)," ");
        ret[stridx] = ' ';
      }

      else if(strcmp(nzpBits,"000") != 0) //if BRN, BRZ, BRP, etc...
      {
        for(int i = 0; i<strlen(nzpBits); i++) //adds any combo of N,Z, or P.
        {
          if(nzpBits[i] == '1')
          {
            ret[stridx] = nzp[i];
            stridx++;
          }
        }
      }

      stridx = tmp;


      if(b7 == 1)
      {
        ret[stridx] = '-';
        stridx++;
      }

      char* brpcoffset9 = intToBaseX(pcoffset9signed,10);

      stridx += strcpylen(ret+stridx,brpcoffset9);

      free(brpcoffset9);
      free(nzpBits);
      break;

    case 1: //ADD, done

      stridx += strcpylen(ret+stridx,"R"); //R?

      int addDR = (instruction & 0x0E00) >> 9; //get ---- XXx- ---- ----

      ret[stridx] = LETTER_DICT[addDR]; //10 -> 'A'
      stridx++;

      stridx += strcpylen(ret+stridx, ", R");

      int addSR1 = (instruction & 0x01C0) >> 6; //get ---- ---X XX-- ----

      ret[stridx] = LETTER_DICT[addSR1]; //11 -> 'B'
      stridx++;

      stridx += strcpylen(ret+stridx, ", ");

      if((instruction & 0x0020)) //if ---- ---- --X- ---- == 1, we want to use imm5. otherwise, use SR2.
      {
        if(bB == 1)//if imm5 negative
        {
          ret[stridx] = '-';
          stridx++;
        }

        char* addimm5c = intToBaseX(imm5signed,10);

        stridx += strcpylen(ret+stridx, addimm5c);
        free(addimm5c);

      }
      else
      {
        stridx += strcpylen(ret+stridx, "R");
        int addsr2 = instruction & 0x0007; //get ---- ---- ---- -###

        ret[stridx] = LETTER_DICT[addsr2];
        stridx++;
      }

      break;

    case 2: //LD, done
      stridx += strcpylen(ret+stridx, "R");

      char* ldDR = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx, ldDR);

      free(ldDR);

      stridx += strcpylen(ret+stridx, ", ");

      if(b7 == 1)
      {
        stridx += strcpylen(ret+stridx, "-");
      }

      char* ldpcoffset9 = intToBaseX(pcoffset9signed,10);

      stridx += strcpylen(ret+stridx, ldpcoffset9);
      free(ldpcoffset9);


      break;

    case 3: //ST, done
      ;
      stridx += strcpylen(ret+stridx,"R");
      char* stSRc = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx,stSRc);

      free(stSRc);

      stridx += strcpylen(ret+stridx,", ");

      if(pcoffset9signed < 0)
      {
        stridx += strcpylen(ret+stridx, "-");
      }

      char* stpcoffset9 = intToBaseX(pcoffset9signed,10);

      stridx += strcpylen(ret+stridx, stpcoffset9);
      free(stpcoffset9);


      break;

    case 4: //JSR or JSRR, done

      if(b4 == 1) //---- #--- ---- ---- flag for "are we JSR?"
      {//using pcoffset11


        if(b5 == 1) //pcoffset11
        {
          ret[stridx] = '-';
          stridx++;
        }
        char* jsrpcoffset11 = intToBaseX(pcoffset11signed,10);

        stridx += strcpylen(ret+stridx, jsrpcoffset11);
        free(jsrpcoffset11);

      }
      else//we're JSRR, using baseR
      {
        ret[3] = 'R';

        ret[stridx] = 'R';
        stridx++;

        char* jsrbaser = intToBaseX(b79,10);

        stridx += strcpylen(ret+stridx, jsrbaser);
        free(jsrbaser);
      }

      break;

    case 5: //AND, done
      stridx += strcpylen(ret+stridx,"R");

      char* andDR = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx,andDR);
      free(andDR);

      stridx += strcpylen(ret+stridx,", R");

      char* andSR = intToBaseX(b79,10);

      stridx += strcpylen(ret+stridx, andSR);
      free(andSR);

      stridx += strcpylen(ret+stridx, ", ");

      if(bA == 1)//if ---- ---- --#- ---- ==1, we're using imm5. else, use SR2.
      {
        tmp = bBF;

        if(tmp > 15)//if neg
        {
          // printf("tmp was %d",tmp );
          // stridx += strcpylen(ret+stridx, "hi");
          // printf("tmp, %d, is a negative 2's comp number\n",tmp);
          tmp = -((~tmp)+33); //it just works man

          // printf(", now it's %d\n", tmp);

          ret[stridx] = '-';
          stridx++;

        }

        char* andimm5 = intToBaseX(tmp,10);

        stridx += strcpylen(ret+stridx, andimm5);
        free(andimm5);


        // char* andimm5c =
      }
      else//using SR2
      {

        stridx += strcpylen(ret+stridx, "R");
        tmp = bDF;

        char* andSR2 = intToBaseX(bDF,10);

        stridx += strcpylen(ret+stridx,andSR2);
        free(andSR2);

      }

      break;

    case 6: //LDR, done

      stridx += strcpylen(ret+stridx,"R");

      char* ldrDR = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx,ldrDR);
      free(ldrDR);

      stridx += strcpylen(ret+stridx, ", R");

      char* ldrbaser = intToBaseX(b79,10);

      stridx += strcpylen(ret+stridx, ldrbaser);
      free(ldrbaser);

      stridx += strcpylen(ret+stridx, ", ");

      char* ldroffset6 = intToBaseX(offset6signed,10);

      if(bA == 1)
      {
        ret[stridx] = '-';
        stridx++;
      }

      stridx += strcpylen(ret+stridx, ldroffset6);
      free(ldroffset6);



      break;

    case 7: //STR, done

      stridx += strcpylen(ret+stridx,"R");

      char* strSR = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx,strSR);
      free(strSR);

      stridx += strcpylen(ret+stridx,", R");

      char* strBaseR = intToBaseX(b79,10);

      stridx += strcpylen(ret+stridx,strBaseR);
      free(strBaseR);

      stridx += strcpylen(ret+stridx,", ");
      ret[stridx] = ' ';

      char* stroffset6 = intToBaseX(offset6signed,10);
      if(b7 == 1)
      {
        ret[stridx] = '-';
        stridx++;
      }

      stridx += strcpylen(ret+stridx, stroffset6);
      free(stroffset6);

      break;

    case 8: //RTI, done
      //i have no fucking idea what to print.
      // stridx += strcpylen(ret+stridx,"wat i print here");
      break;

    case 9: //NOT, done

      stridx += strcpylen(ret+stridx,"R");

      char* notDR = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx,notDR);
      free(notDR);

      stridx += strcpylen(ret+stridx,", R");

      char* notSR = intToBaseX(b79,10);

      stridx += strcpylen(ret+stridx,notSR);
      free(notSR);

      break;

    case 10: //LDI, done


      stridx += strcpylen(ret+stridx,"R");

      char* ldiDR = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx,ldiDR);
      free(ldiDR);

      stridx += strcpylen(ret+stridx,", ");

      char* ldipcoffset9 = intToBaseX(pcoffset9signed,10);

      if(b7 == 1)
      {
        ret[stridx] = '-';
        stridx++;
      }

      stridx += strcpylen(ret+stridx,ldipcoffset9);
      free(ldipcoffset9);

      break;

    case 11: //STI, done

      stridx += strcpylen(ret+stridx,"R");

      char* stiSR = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx,stiSR);
      free(stiSR);

      stridx += strcpylen(ret+stridx, ", ");

      if(b7 == 1)
      {
        ret[stridx] = '-';
        stridx++;
      }

      char* stipcoffset9 = intToBaseX(pcoffset9signed,10);

      stridx += strcpylen(ret+stridx,stipcoffset9);
      free(stipcoffset9);

      break;

    case 12: //JMP, done


      ret[stridx] = 'R';
      stridx++;

      char* jmpbaser = intToBaseX(b79,10);

      stridx += strcpylen(ret+stridx,jmpbaser);
      free(jmpbaser);


      break;

    case 13: //reserved, done. initiate self destruct...
      // stridx += strcpylen(ret+stridx,"oh shit");
      break;

    case 14: //LEA, done.
      stridx += strcpylen(ret+stridx,"R");

      char* leaDR = intToBaseX(b46,10);

      stridx += strcpylen(ret+stridx,leaDR);
      free(leaDR);

      stridx += strcpylen(ret+stridx,", ");

      char* leapcoffset9 = intToBaseX(pcoffset9signed,10);

      if(pcoffset9signed < 0)
      {
        stridx += strcpylen(ret+stridx,"-");
      }

      stridx += strcpylen(ret+stridx,leapcoffset9);
      free(leapcoffset9);

      break;

    case 15: //TRAP

      ret[stridx] = 'x';
      stridx++;

      tmp = instruction & 0x00FF;

      char* trapchars = intToBaseX(tmp,16);

      stridx += strcpylen(ret+stridx,trapchars);

      // if(strlen(trapchars) < 2)
      // {
      //   ret[stridx] = '0';
      //   stridx++;
      // }



      break;

    default: //done...

      break;
  }

  ret[stridx] = ' '; //rids us of that dumb '\0' char

  free(instructionS);
  free(opcodeS);
  // printf("\n\n\n\n");

  // printf("returning \"%s\"\n",ret);
  return ret;
}

























//pvbx(17,16,'X',5) = "XXX10"
//normally 17b10 is 10b16...
char* prettify_val_baseX(int val, int base, char padding, int columns)
{ //0xF0C, 16, '0', 5 -> 00F0C...

  if(val < 0){val = -val;} //fixes crash....

  char* small_val = intToBaseX(val, base);

  char* pretty_val = malloc(columns);
  initCharArr(pretty_val,columns);
  int missingVals = columns - strlen(small_val);

  for(int i=0; i<missingVals; i++)
  {
    pretty_val[i] = padding; //00 ???
  }

  for(int i=missingVals; i<columns; i++)
  {
    pretty_val[i] = small_val[i-missingVals]; //?? F0C
  }

  free(small_val);
  // printf("returning %s\n",pretty_val);
  return pretty_val;
}

int twosCompIntToUnsigned(int twoscomp, int columns)//-3763,16 -> 61773
{
  if(twoscomp >= 0){return twoscomp;}

  int place = 0;
  while(ipow(2,place) < ipow(2,columns))
  {
    place++;
  }

  int maxNum = ipow(2,place);

  return maxNum + twoscomp;
  //i.e. 2sCompIntToUnsigned(-1,16) -> 2^16 + (-1) aka 65536-1! woooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
}

int charToNum(char character) //'A' -> 10
{
  for(int i=0; i<strlen(LETTER_DICT); i++)
  {
    if(LETTER_DICT[i] == character) //"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[13] = "D"
    {
      return i;
    }
  }
  return 0;
}

int strToBaseX(char* aString, int base) //"FFFF",16 -> 65535 or "4D2",16 -> 1234
{
  char *str = aString;
  long int ret = 0;
  int ourNum = 0;

  for(int i=0; i<strlen(str); i++)
  {
    ourNum = charToNum(str[i]);

    while(ourNum > 0)
    {
      ret += (ipow(base,(strlen(str)-i-1))); //ugly magic
      ourNum--;
    }
  }
  return ret;
}


char* intToBaseX(int value, int base) //1234,16 -> "4D2"
{ // 1234 -> 4D2
  bool neg = false;
  if(value == 0)
  {
    char* ret = malloc(2);
    strcpy(ret,"0\0");
    return ret;
  }

  if(value < 0)
  {
    neg = true;
    value = -value;
  }


	int converted_len, to_convert;
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
      // printf("%d^%d is %d\n",base,i,(ipow(base,i)));


			//printf("About to %d - %d \n",to_convert,(ipow(base,i)));

			to_convert += -ipow(base,i); //take away our 2^0 or 2^1 or 2^2 etc
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





























char* prettify_mempos(int addr, int val, CPU *cpu) //DONT FUCKING FORGET TO USE FREE() YOU SILLY BOYYYYY
{
  //[FFFF]  A13D  |  -24259  |
  //[XXXX]  YYYY  |  ?ZZZZZ  |
  int stridx = 0;

  char charAddr[7] = {0};
  char charVal[7] = {0};
  sprintf(charAddr,"%d",addr);
  sprintf(charVal,"%d",val);

  int bufsize = 28;
  int unsAddr = twosCompIntToUnsigned(addr,16);
  int unsVal = twosCompIntToUnsigned(val,16);

  unsigned int int1 = logicRShift((val & 0xF000), 12); //HERE ???? ???? ????
  unsigned int int2 = (val & 0x0F00) >> 8;             //???? HERE ???? ????
  unsigned int int3 = (val & 0x00F0) >> 4;             //???? ???? HERE ????
  unsigned int int4 = (val & 0x000F) >> 0;             //???? ???? ???? HERE

  char* hexAddr = prettify_val_baseX(unsAddr,16,'0',4);
  char* hexVal = prettify_val_baseX(unsVal,16,'0',4);


  char* spacerA = "  ";
  char* spacerB = "  |  ";
  char* spacerAB = "  | ";
  char* spacerC = "|  ";


  //printf("signed    mem[%d] = %d\n",addr,val);
  //printf("unsigned  mem[%d] = %d\n",unsAddr, unsVal);
  //printf("hex       mem[%s] = %s\n",hexAddr, hexVal);
  //printf("charSgnd  mem[%s] = %s\n",charAddr,charVal);

  char *pretty_thingy = malloc(bufsize); //47+1

  fillCharArr(pretty_thingy, bufsize, ' ','\0');


  stridx += strcpylen((pretty_thingy+stridx),"  | x");

  if(cpu->pc == addr)
  {
    // pretty_thingy[2] = 16; //some triangle character
    pretty_thingy[2] = '>';
  }

  strcpylen(pretty_thingy+stridx,hexAddr); //hex address
  stridx += 4; //padding

    stridx += strcpylen((pretty_thingy+stridx),":   ");

  strcpylen(pretty_thingy+stridx, hexVal); //hex value
  stridx += 4;

  stridx += strcpylen(pretty_thingy+stridx, "  | ");
  pretty_thingy[stridx] = ' ';

  if(val >= 0){stridx++;}

  int tmp = 0;


  for(int i=0; i<strlen(charVal); i++)
  {
    // printf("adding %c\n",charVal[i]);
    pretty_thingy[stridx] = charVal[i];
    stridx++;
    tmp++;
  }

  while(tmp<6) //if our _10num is small
  {
    pretty_thingy[stridx] = ' ';
    stridx++;
    tmp++;
  }

  if(val<0)
  {
    pretty_thingy[stridx] = ' ';
    stridx++; //to compensate for the minus sign
  }


  tmp = 0;

  free(hexAddr);
  free(hexVal);


  pretty_thingy[bufsize] = '\0';
  // printf("ret='%s'\n",pretty_thingy);
  return pretty_thingy;
}

void setConditionCode(CPU *cpu, int num)
{
  if(num < 0)
  {
    cpu->cc = '<';
    return;
  }

  if(num == 0)
  {
    cpu->cc = '=';
    return;
  }

  if(num > 0)
  {
    cpu->cc = '>';
    return;
  }
}

char cctochar(char cc)
{
  if(cc == '<'){return 'N';}
  if(cc == 'N'){return '<';}
  if(cc == '='){return 'Z';}
  if(cc == 'Z'){return '=';}
  if(cc == '<'){return 'P';}
  if(cc == 'P'){return '>';}
  

  return '?';
}
