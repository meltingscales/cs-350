	// (Remove this entire comment block:
// Instructions:
// Rename this skeleton to Lab6_YourName_Section.c and complete
// the program.	 Note: This skeleton doesn't execute well.)

// *** Your name and section (replace this with the correct information ***

// CS 350, Fall 2016
// Lab 6: SDC Simulator, part 1
//
// Illinois Institute of Technology, (c) 2016, James Sasaki

#include <stdio.h>
#include <stdlib.h>	  // For error exit()
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void printIf(char* stringy, int booly) //self explanatory. prints stringy if booly == 1...
{
	if(booly){printf("%s",stringy);}
}


// CPU Declarations -- a CPU is a structure with fields for the
// different parts of the CPU.
//
	typedef short int Word;		 // type that represents a word of SDC memory
	typedef unsigned char Address;	 // type that represents an SDC address

	#define MEMLEN 100
	#define NREG 10
	#define DEBUG 0

	static const char* INSTRUCTION_NAMES[] = {"HALT","LD","ST","ADD","NEG","LDM","ADDM","BR","BRC"}; //0,1,2,3,4,5,6,7,8
	static const int INSTRUCTION_INFO[]  = {     0,    3,   3,   3,    1,    7,     7,    2,   7 };
	//first bit denotes that it uses register, second indicates it uses the memory decimals, 3rd bit indicates it uses sign.
	//to do logic, AND it with 1/2/4... if it's b1&b2, b2 contains b1.
	//useful for fancy printing.

	static const char* PRINT_INSTRUCTION_NAMES[] = {"GETC", "OUT", "PUTS", "DMP", "MEM", "NOP","NOP","NOP","NOP","NOP"}; //90,91,92,93,94,95-99
	static const int PRINT_INSTRUCTION_PARAMETERS[] = {0,      0,      2,     0,     0,     0,    0,    0,    0,    0   };
	//see above words



	struct CPU {
		Word mem[MEMLEN];
		Word reg[NREG];	     // Note: "register" is a reserved word
		Address pc;	     // Program Counter
		int running;	     // running = 1 iff CPU is executing instructions
		Word ir;	     // Instruction Register
		int instr_sign;	     //	  sign of instruction
		int opcode;	     //	  opcode field
		int reg_R;	     //	  register field
		int addr_MM;	     //	  memory field
	};
	typedef struct CPU CPU;


// Prototypes [note the functions are also declared in this order]
//

	int main(int argc, char *argv[]);
	void initialize_control_unit(CPU *cpu);
	void initialize_memory(int argc, char *argv[], CPU *cpu);
	FILE *get_datafile(int argc, char *argv[]);
	char *resolveOpcode(int opcode);
	char *resolveOpcodeInfo(int opcodeButLonger);

	void dump_control_unit(CPU *cpu);
	void dump_memory(CPU *cpu);
	void print_instr(int instr);
	void dump_registers(CPU *cpu);

	int read_execute_command(CPU *cpu);
	int execute_command(char *cmd_char, CPU *cpu);
	void help_message(void);
	void many_instruction_cycles(int nbr_cycles, CPU *cpu);
	void one_instruction_cycle(CPU *cpu);

	void exec_HLT(CPU *cpu); 	//0
	void exec_LD(CPU *cpu);		//1
	void exec_ST(CPU *cpu);		//2
	void exec_ADD(CPU *cpu);	//3
	void exec_NEG(CPU *cpu);	//4
	void exec_LDM(CPU *cpu);	//5
	void exec_ADDM(CPU *cpu); // +6
	void exec_SUBM(CPU *cpu); // -6
	void exec_BR(CPU *cpu);		//7
	void exec_BRC(CPU *cpu);	//8

	void exec_GETC(CPU *cpu);	//90
	void exec_OUT(CPU *cpu);	//91
	void exec_PUTS(CPU *cpu); //92
	void exec_DMP(CPU *cpu);  //93
	void exec_MEM(CPU *cpu);  //94

// Main program: Initialize the cpu, and read the initial memory values.
// Then run the command loop to let the user execute the program.
//
int main(int argc, char *argv[]) {
	printf("SDC Simulator P2. Henry Post, hpost@hawk.iit.edu.\n");
	CPU cpu_value, *cpu = &cpu_value;

	initialize_control_unit(cpu);
	dump_control_unit(cpu);
	initialize_memory(argc, argv, cpu);
	dump_memory(cpu);

	// Run the command loop
	//
	char *prompt = "> ";
	printf("\nBeginning execution; type h for help.\n", prompt);

	int done = 0;

	while (!done)
	{
		printf("%s", prompt);
		done = read_execute_command(cpu);
	}
	return 0;
}


void initialize_control_unit(CPU *cpu)
{
	cpu->pc = 00; //program counter
	cpu->ir = 0000;
	cpu->instr_sign = 1; //magic from Sasaki-san...?
  cpu->running = 1;

  for(int i=0; i < NREG; i++)
  {
    cpu->reg[i] = 0;
  }
}



void initialize_memory(int argc, char *argv[], CPU *cpu)
{
  FILE *datafile = get_datafile(argc, argv);

				#define DATA_BUFFER_LEN 256
        char buffer[DATA_BUFFER_LEN];

        // Will read the next line (words_read = 1 if it started
        // with a memory value). Will set memory location loc to
        // value_read

        int value_read, words_read, loc = 0, done = 0;

        char *read_success; // NULL if reading in a line fails.
        read_success = fgets(buffer, DATA_BUFFER_LEN, datafile);

        while (read_success != NULL && !done && (cpu->running == 1))
        {
          words_read = sscanf(buffer, "%d", &value_read); //sets third arg, aka &value_read to a char sequence

          if(loc <= MEMLEN) //loc is within range
          {
            if(abs(value_read) <= 9999) //if not sentinel
            {
              cpu->mem[loc] = value_read; //set cpu.mem[loc] to our read val
            }
            else //sentinel encountered
            {
              done = true;
              break;
            }
          }
          else //loc not in range
          {
            printf("Tried to put instructions at mem[%d], which is too large!\n",loc);
            done = true; //to exit loop
            break;
          }
          read_success = fgets(buffer, DATA_BUFFER_LEN, datafile); //get next line
          loc++;
        }

        while (loc < MEMLEN)
        {
                cpu->mem[loc++] = 0; //to set unused slots to zero to avoid random shit
        }
}

// Read a simulator command from the keyboard (q, h, ?, d, number,
// or empty line) and execute it.  Return true if we hit end-of-input
// or execute_command told us to quit.	Otherwise return false.
//
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

// Execute a nonnumeric command; complain if it's not 'h', '?',
// 'd', 'q' or '\n'. Return true for the q command, false otherwise.
//
int execute_command(char *cmd_char, CPU *cpu)
{
	cmd_char[1] = '\0'; //to remove any extra crap...
	// cmd_char = "holy shit i want help"
	// cmd_char = "h\0ly shit i want help"
	//printf("%s",cmd_char) -> "h"

	if(DEBUG){printf("we are executing %s\n",cmd_char);}

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

// Print standard message for simulator help command ('h' or '?')
//
void help_message(void)
{
	printf("    h: help		q: quit		d: dump CPU\n    Otherwise, enter a number (or nothing) to execute X CPU cycles.\n");
}

// Execute a number of instruction cycles.  Exceptions: If the
// number of cycles is <= 0, complain and return; if the CPU is
// not running, say so and return; if the number of cycles is
// insanely large, warn the user and substitute a saner limit.
//
// If, as we execute the many cycles, the CPU stops running,
// stop and return.
//
void many_instruction_cycles(int nbr_cycles, CPU *cpu)
{
	
	printf("HI I WAS GIVEN %d !!!\n",nbr_cycles);
	int INSANE_NBR_CYCLES = 100;

	if(cpu->running)
	{
		if(nbr_cycles > 0 && nbr_cycles <= INSANE_NBR_CYCLES)
		{
			for(int i=0; i<nbr_cycles; i++)
			{
				one_instruction_cycle(cpu);
			}
		}
		else
		{
			printf("Was given %d cycles, it's over %d, which is too large!\n",nbr_cycles,INSANE_NBR_CYCLES);
		}
	}
	else
	{
		printf("CPU is not running, will not execute any instruction cycles!\n");
	}
}

// Execute one instruction cycle
//
void one_instruction_cycle(CPU *cpu)
{
	// If the CPU isn't running, say so and return.
	// If the pc is out of range, complain and stop running the CPU.
	//
	// *** STUB ****

	// Get instruction and increment pc
	//
	// For printing purposes, we'll save the location of
	// the instruction (the pc before the increment).
	//
	if(cpu->running == 1 && cpu->pc < MEMLEN - 1)
	{
		int instr_loc = cpu -> pc; //get instruction location
		cpu -> ir = cpu -> mem[cpu -> pc]; //set instruction register to where program counter references to the memory...
		(cpu -> pc)++; //increment program counter to get ready to read next instruction
	}
	else
	{
		if(!cpu->running)
		{
			printf("CPU is not running, will not execute any instruction cycles!\n");
			return;
		}

		if(cpu->pc >= MEMLEN)
		{
			printf("Hit end of memory. Stopping.\n");
			cpu->running = 0;
			return;
		}

		printf("No clue...");
		return;
	}

	// Decode instruction into opcode, reg_R, addr_MM, and instruction sign
	//
	// *** STUB ****
	int operation = cpu->mem[cpu->pc-1];

	cpu->opcode = 		abs(operation / 1000); //chop off last three
	cpu->reg_R = 			abs((operation / 100) % 10); //just third. chop off last two, first one.
	cpu->addr_MM = 		(operation % 100); //last two

	if(operation < 0)
			 {cpu->instr_sign = -1;}
	else {cpu->instr_sign =  1;}


	if(DEBUG){printf("\nFULL: % 04d  OPCOD: %2d  REG_R: %d  ADRMM: %3d  ISIGN: %2d\n",operation,cpu->opcode,cpu->reg_R,cpu->addr_MM,cpu->instr_sign);}

	//this prints our just-executed opcode and info 'bout it.
	char *freemeplease = resolveOpcodeInfo(operation);


	printf("%4s %s\n",resolveOpcode((cpu->opcode*10)+cpu->reg_R),freemeplease);

	free(freemeplease); //my code sucks ass



	switch (cpu -> opcode)
	{

		//printf("i am fuck, destroyer of up: %d",cpu->opcode);

		case 0: printIf("Executing HLT... \n",DEBUG);	exec_HLT(cpu); 	break;
		case 1:	printIf("Executing LD...  \n",DEBUG);	exec_LD(cpu);		break;
		case 2: printIf("Executing ST...  \n",DEBUG);	exec_ST(cpu);		break;
		case 3: printIf("Executing ADD... \n",DEBUG);	exec_ADD(cpu);	break;
		case 4: printIf("Executing NEG... \n",DEBUG);	exec_NEG(cpu);	break;
		case 5: printIf("Executing LDM... \n",DEBUG);	exec_LDM(cpu);	break;

		case 6: if(cpu->instr_sign > 0){printIf("Executing ADDM...\n",DEBUG);	exec_ADDM(cpu);} 			//+6
												 			 else{printIf("Executing SUBM...\n",DEBUG);	exec_SUBM(cpu);} break;//-6

		case 7: printIf("Executing BR...  \n",DEBUG);	exec_BR(cpu);		break;

		case 8: if(cpu->instr_sign > 0){printIf("Executing BRGE...\n",DEBUG);	exec_BRGE(cpu);}
															 else{printIf("Executing BRLE...\n",DEBUG);	exec_BRLE(cpu);} break;

		case 9:		 //9x
			if(cpu->reg_R == 0){printIf("Executing GETC...\n",DEBUG);	exec_GETC(cpu);	break;} //90
			if(cpu->reg_R == 1){printIf("Executing OUT... \n",DEBUG);	exec_OUT(cpu);	break;} //91
			if(cpu->reg_R == 2){printIf("Executing PUTS...\n",DEBUG);	exec_PUTS(cpu);	break;} //92
			if(cpu->reg_R == 3){printIf("Executing DMP... \n",DEBUG);	exec_DMP(cpu);	break;} //93
			if(cpu->reg_R == 4){printIf("Executing MEM... \n",DEBUG);	exec_MEM(cpu);	break;} //94
			break;

	default: printf("Bad opcode!? %d\n", cpu -> opcode);
	}
}

// Execute the halt instruction (make CPU stop running)
//
void exec_HLT(CPU *cpu) //0, works.
{
	cpu -> running = 0;
}

void exec_LD(CPU *cpu) //1, works.
{
	cpu->addr_MM = abs(cpu->addr_MM); //want it to be pos

	if(DEBUG){printf("About to set cpu->reg[%d] to cpu->mem[%d], aka %d\n",cpu->reg_R,cpu->addr_MM,cpu->mem[cpu->addr_MM]);}
	cpu->reg[cpu->reg_R] = cpu->mem[cpu->addr_MM]; //Reg[R] = Mem[MM]...
}

void exec_ST(CPU *cpu) //2, probably works.
{
	cpu->addr_MM = abs(cpu->addr_MM); //want it to be pos

	if(DEBUG){printf("About to set cpu->mem[%d] to cpu->reg[%d], aka %d\n",cpu->addr_MM,cpu->reg_R,cpu->reg[cpu->reg_R]);}
	cpu->mem[cpu->addr_MM] = cpu->reg[cpu->reg_R];
}

void exec_ADD(CPU *cpu) //3
{
	cpu->addr_MM = abs(cpu->addr_MM); //want it to be pos

	if(DEBUG){printf("About to set cpu->reg[%d] to itself plus cpu->mem[%d]\n",cpu->reg_R,cpu->addr_MM);}
	cpu->reg[cpu->reg_R] += cpu->mem[cpu->addr_MM];
}

void exec_NEG(CPU *cpu) //4
{
	if(DEBUG){printf("About to set cpu->reg[%d] to -1 * cpu->reg[%d]\n",cpu->reg_R,cpu->reg_R);}
	cpu->reg[cpu->reg_R] = -(cpu->reg[cpu->reg_R]);
}

void exec_LDM(CPU *cpu) //5
{
	if(DEBUG){printf("About to set cpu->reg[%d] to %d\n",cpu->reg_R,cpu->addr_MM);}
	cpu->reg[cpu->reg_R] = cpu->addr_MM;
}

void exec_ADDM(CPU *cpu) //6
{
	cpu->addr_MM = abs(cpu->addr_MM); //want it to be pos

	if(DEBUG){printf("About to set cpu->reg[%d] to itself plus %d\n",cpu->reg_R,cpu->addr_MM);}
	cpu->reg[cpu->reg_R] += cpu->addr_MM;
}

void exec_SUBM(CPU *cpu) //-6
{
	cpu->addr_MM = -abs(cpu->addr_MM); //we want it to be negative

	if(DEBUG){printf("About to set cpu->reg[%d] to itself minus %d\n",cpu->reg_R,-cpu->addr_MM);}
	cpu->reg[cpu->reg_R] += cpu->addr_MM;
}

void exec_BR(CPU *cpu) //7
{
	cpu->addr_MM = abs(cpu->addr_MM); //want it to be pos

	if(DEBUG){printf("About to set cpu->PC to %d\n",cpu->addr_MM);}
	cpu->pc = cpu->addr_MM;
}

void exec_BRGE(CPU *cpu) //8
{
	cpu->addr_MM = abs(cpu->addr_MM); //want it to be pos

	if(DEBUG){printf("Branch: Is cpu->reg[%d], aka %d, >= 0? ... ",cpu->reg_R,cpu->reg[cpu->reg_R]);}

	if(cpu->reg[cpu->reg_R] >= 0)
	{
		if(DEBUG){printf("It is!\nWe're setting cpu->pc to %d!\n",cpu->addr_MM);}
		cpu->pc = cpu->addr_MM;
	}
	else
	{
		if(DEBUG){printf("It isn't. Doing nothing...\n");}
	}
}

void exec_BRLE(CPU *cpu) //-8
{
	cpu->addr_MM = abs(cpu->addr_MM); //want it to be pos

	if(DEBUG){printf("Branch: is cpu->reg[%d], aka %d. <= 0? ... ",cpu->reg_R,cpu->reg[cpu->reg_R]);}

	if(cpu->reg[cpu->reg_R] <= 0)
	{
		if(DEBUG){printf("It is!\nWe're setting cpu->pc to %d!\n",cpu->addr_MM);}
		cpu->pc = cpu->addr_MM;
	}
	else
	{
		if(DEBUG){printf("It isn't. Doing nothing...\n");}
	}
}

void exec_GETC(CPU *cpu) //90
{
	printf("Setting cpu->reg[0] to whatever you type in here: ");
	char tmp;
	scanf("%c",&tmp);

	cpu->reg[0] = tmp;
	if(DEBUG){printf("We just set cpu->reg[0] to %d, or '%c'!\n",cpu->reg[0],cpu->reg[0]);}
}

void exec_OUT(CPU *cpu) //91
{
	if(DEBUG){printf("Printing cpu->reg[0] as a char...\n");}
	printf("%c\n",(char)cpu->reg[0]);
}

void exec_PUTS(CPU *cpu) //92
{
	if(DEBUG){printf("Printing a string from MM, MM+1, ... ,until we hit a zero.\n");}
	int temp = cpu->addr_MM;
	while(cpu->mem[temp] != 0) //while we're not hitting a zero...
	{
		printf("%c\n",cpu->mem[temp]);//print number as ascii
		temp++;
	}
}

void exec_DMP(CPU *cpu) //93
{
	dump_control_unit(cpu);
}

void exec_MEM(CPU *cpu) //94
{
	dump_memory(cpu);
}




//don't look below pls, ugly





































FILE *get_datafile(int argc, char *argv[])
{
  char *default_datafile_name = "default.sdc";
  char *datafile_name = NULL;

  if(argv[1] == NULL)
  {
    datafile_name = default_datafile_name;
    if(DEBUG){printf("Datafile name set to %s, no name provided.\n",default_datafile_name);}
  }
  else
  {
    datafile_name = argv[1];
    if(DEBUG){printf("Datafile name set to %s\n",argv[1]);}
  }

  FILE *datafile = fopen(datafile_name, "r");

  if(datafile == NULL)
  {
    printf("Opening datafile failed, exiting.");
    exit(EXIT_FAILURE);
  }

  return datafile;
}

void dump_control_unit(CPU *cpu)
{
  printf("Dumping control unit...\n");
  printf("     PC  %d\n",cpu->pc);
  printf("     IR  %d\n",cpu->ir);
  printf("Running? %d\n",cpu->running);
  dump_registers(cpu);
}

char *resolveOpcodeInfo(int instr)
{
  int first = abs(instr / 1000);
  int second = abs((instr / 100) % 10);
  int lastTwo = abs(instr % 100);
  int sign = (instr / abs(instr));


  char ret1[3] = {0}; //RX\0
  char ret2[4] = {0}; //-XX\0
  int opcodeInfo;

  if(first < 9)
  {opcodeInfo = INSTRUCTION_INFO[first];}
  else
  {opcodeInfo = INSTRUCTION_INFO[second];}
  //make a pretty string....

  if(opcodeInfo & 1) //registry flag == true
  {
    sprintf(ret1,"R%d",second);
  }
  if((opcodeInfo & 2) && (opcodeInfo & 4)) //uses memory flag == true AND care'boutSign == true
  {
    sprintf(ret2,"% 3d",(sign*lastTwo));
  }
  else if(opcodeInfo & 2) //memflag but idk about +-
  {
    sprintf(ret2,"% 3d",(lastTwo));
  }

  char *ret12 = malloc(sizeof(char) * 7);

  if(ret1[0] != 0 &&  ret2[0] != 0) //we gots us both, put a friggin' comma!!!!
  {
    sprintf(ret12,"%s,%s",ret1,ret2);
  }
  else //no comma
  {
    sprintf(ret12,"%s%s",ret1,ret2+1); //bro thx based pointer jesus #eeeeeeeeeeeeeeeeeeeeeeeeevvvvvvvvvvvvvvvvvvvvvvvvaaaaaaaaaaaaaaaaaaaaaaaaaaaaannnnnnnnnnnnnnnnnnn
  }                                       ///////magnets///////////

return ret12;

}

char *resolveOpcode(int opcode)
{

  if((opcode >= 0) && (opcode <= 8)) //use our nice list...
  {
    return INSTRUCTION_NAMES[opcode];
  }
	else if((opcode/10 >= 0) && (opcode/10 <= 8))
	{
		return INSTRUCTION_NAMES[opcode/10];
	}
  else if((opcode >= 90) && (opcode <= 99)) //use our second nice list...
  {
    return PRINT_INSTRUCTION_NAMES[(opcode - 90)];
  }
  return "WTF?";
}

void dump_registers(CPU *cpu)
{
  char *regStr[(7*10)] = {0}; //for spaces and chars blah

  for(int i = 0; i <= 9; i++)
  {
      sprintf((regStr + (7*i)),"R%d: %02d",i,cpu->reg[i]);
  }

for(int i = 0; i <= 9; i++)
{
  if(i == 5)
  {
    printf("\n");
  }
  printf("%s ",(regStr + (7*i)));
}
printf("\n\n");

}


void dump_memory(CPU *cpu)
{
  if(cpu->running == true)
  {
    printf("Memory: @Loc, value, instr (nonzero values only):\n");
    for(int loc = 0; loc<MEMLEN; loc++)
    {
      if(cpu->mem[loc] != 0) //if nonzero
      {
        char *resolvedOpcode[12]; //smonk the big one. also add null or use sprintf...
        char *resolvedOpcode2[99];

        int sign =       (cpu->mem[loc] / abs(cpu->mem[loc]));
        int firstTwo =   (cpu->mem[loc] / 100);
        int lastTwo =    (cpu->mem[loc] % 100);
        int firstOne =   (cpu->mem[loc] / 1000); //all of these work. woo.
        int secondOne = ((cpu->mem[loc] / 100) % 10);
        //now we handle the opcode!!!


        if(DEBUG){
        printf("\n\nAbout to handle %d\n",cpu->mem[loc]);
        printf("firstTwo  : %d__\n",firstTwo);
        printf("lastTwo   : __%d\n",lastTwo);
        printf("firstOne  : %d___\n",firstOne);
        printf("secondOne : _%d__\n",secondOne);}



        if((abs(firstOne) >= 0) && (abs(firstOne) <= 8)) //first eight
        {
          sprintf(resolvedOpcode,"%s",resolveOpcode(abs(firstOne))); //opcode is 0...8
        }
        else if((abs(firstTwo) >= 90) && (abs(firstTwo) <= 99))
        {
          sprintf(resolvedOpcode,"%s",resolveOpcode(abs(firstTwo))); //opcode is 90..99
        }
        else
        {
          sprintf(resolvedOpcode,"%s","WTF             ");
        }

        char *info = resolveOpcodeInfo(cpu->mem[loc]);

        sprintf(resolvedOpcode2,"%5s  % 0s",resolvedOpcode,info);

        free(info); //because we do string splicing


        printf("@ %02d   % 05d   % 2s\n",
        loc,
        cpu->mem[loc],
        resolvedOpcode2);
      };
    }

    printf("End of memory dump.\n");

  }
}






















/*

this was cut out of if(isdigit(read_success))...

{
	printf("Length of read_succes is %d.\n",strlen(read_success));
	for(int i = 0; i<strlen(read_success)-1; i++) //loop through string[0,#str-1]
	{
		printf("i is %d",i);
		if(isdigit(*read_success+i) && !done && is_number) //if read_suc[i] is digit
		{
			printf("Adding %d to our nbr_cycles...\n",((int)pow(10,(strlen(read_success)-i-2)) * ((read_success[i]) - '0'))); //10^n * number

			nbr_cycles += (((int)pow(10,(strlen(read_success)-i-2)) * ((read_success[i]) - '0'))); //tack on to our nbr_cycles
			printf("nbr_cycles = %d\n",nbr_cycles);
		}
		else
		{
			printf("Was given mixed numbers and decimals! Not executing...");
			nbr_cycles = 0;
			is_number = 0;
			break;
		}
	}
	nbr_cycles += -23; //i dont fucking know why


*/
