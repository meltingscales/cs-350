// *** Henry Post, CS350-01 (replace this with the correct information ***

// CS 350, Fall 2016
// Lab 5: SDC Simulator, part 1
//
// Illinois Institute of Technology, (c) 2016, James Sasaki

#include <stdio.h>
#include <stdlib.h>       // For error exit()
#include <stdbool.h>
#include <string.h>
#include <math.h>

static bool DEBUG = false;
static const char* INSTRUCTION_NAMES[] = {"HALT","LD","ST","ADD","NEG","LDM","ADDM","BR","BRC"}; //0,1,2,3,4,5,6,7,8
static const int INSTRUCTION_INFO[]  = {     0,    3,   3,   3,    1,    7,     7,    2,   7 };
//first bit denotes that it uses register, second indicates it uses the memory decimals, 3rd bit indicates it uses sign.
//to do logic, AND it with 1/2/4... if it's b1&b2, b2 contains b1.
//useful for fancy printing.

static const char* PRINT_INSTRUCTION_NAMES[] = {"GETC", "OUT", "PUTS", "DMP", "MEM", "NOP","NOP","NOP","NOP","NOP"}; //90,91,92,93,94,95-99
static const int PRINT_INSTRUCTION_PARAMETERS[] = {0,      0,      2,     0,     0,     0,    0,    0,    0,    0   };
//see above words


// CPU Declarations -- a CPU is a structure with fields for the
// different parts of the CPU.
//
        typedef short int Word;          // type that represents a word of SDC memory
        typedef unsigned char Address;   // type that represents an SDC address

        #define MEMLEN 100
        #define NREG 10

        struct CPU
        {
          Word mem[MEMLEN];
          Word reg[NREG];      // Note: "register" is a reserved word
          Address pc;          // Program Counter
          int running;         // running = 1 iff CPU is executing instructions
          Word ir;             // Instruction Register
          int instr_sign;      //   sign of instruction
          int opcode;          //   opcode field
          int reg_R;           //   register field
          int addr_MM;         //   memory field
        };

        typedef struct CPU CPU;


// Prototypes [note the functions are also declared in this order]
//
        int main(int argc, char *argv[]);
        void initialize_control_unit(CPU *cpu);
        void initialize_memory(int argc, char *argv[], CPU *cpu);
        FILE *get_datafile(int argc, char *argv[]);

        void dump_control_unit(CPU *cpu);
        void dump_memory(CPU *cpu);
        void print_instr(int instr);
        void dump_registers(CPU *cpu);

void setCharArray(char* arr, char c, int size)
{
  for(int i = 0; i < size; i++)
  {
    arr[i] = c;
  }
  return;
}

void printArr(char* arr, int size)
{
  for(int i = 0; i < size; i++)
  {
    if(arr[i] == '\0')
    {
      printf(" NUL ");
    }
    else
    {
      printf("%c",arr[i]);
    }
  }
}


// Main program: Initialize the cpu, and read the initial memory values ... (add more to this comment in Lab 6)
//
int main(int argc, char *argv[]) {
        printf("SDC Simulator pt 1 skeleton: CS 350 Lab 5\n");
        
        printf("I was passed argc, which is %d.\n",argc);

        for(int i = 0; i < argc; i++)
        {
          printf("argv[%d] = %s\n", i,argv[i]);
        }
        
        
        
        
        CPU cpu_value, *cpu = &cpu_value;
        initialize_control_unit(cpu);
        dump_control_unit(cpu);
        initialize_memory(argc, argv, cpu);
        dump_memory(cpu);

        // That's it for Lab 5
        //
        return 0;
}


// Initialize the control unit (pc, ir, instruction sign,
// running flag, and the general-purpose registers).
//
void initialize_control_unit(CPU *cpu) {

        //printf("Initializing CPU...");
        cpu->running = 1;

        for(int i=0; i<=9; i++)
        {
          cpu->reg[i] = 0;
        }
        // *** STUB ***
}

// Read and dump initial values for memory
//
void initialize_memory(int argc, char *argv[], CPU *cpu)
{
  //printf("Getting datafile...\n");
  FILE *datafile = get_datafile(argc, argv);

  //printf("Initializing memory...\n");

// Buffer to read next line of text into
#define DATA_BUFFER_LEN 256
        char buffer[DATA_BUFFER_LEN];

        // Will read the next line (words_read = 1 if it started
        // with a memory value). Will set memory location loc to
        // value_read

        int value_read, words_read, loc = 0, done = 0;

        char *read_success; // NULL if reading in a line fails.

        read_success = fgets(buffer, DATA_BUFFER_LEN, datafile);
        
        printf("we're taking data from our FILE datafile object and setting BUFFER to each line of it...\n");
        
        while (read_success != NULL && !done && (cpu->running == 1))
        {
          
          printf("buffer = \n\n\'%s\'\n\n",buffer);
          
          // If the line of input begins with an integer, treat
          // it as the memory value to read in.  Ignore junk
          // after the number and ignore blank lines and lines
          // that don't begin with a number.
          words_read = sscanf(buffer, "%d", &value_read);
          //if(DEBUG){usleep(50000);};
          
          /*words_read represents whether or not a digit was found at the beginning*/
          printf("words_read = %d\n",words_read);
          printf("value_read from buffer is %d\n",value_read);
          
          // *** STUB ***
          // if an integer was actually read in, then
          // set memory value at current location to
          // value_read and increment location.  Exceptions: If
          // loc is out of range, complain and quit the loop. If
          // value_read is outside -9999...9999, then it's a
          // sentinel -- we should say so and quit the loop.

          if((loc <= MEMLEN)) //loc is within range
          {
            if((value_read < 9999) && (value_read > -9999)) //if not sentinel
            {
              //if(DEBUG){printf("mem[%d]=%d\n",loc,value_read);}
              if((words_read >= 1))
              {
                cpu->mem[loc] = value_read; //set cpu.mem[loc] to our read val
                loc++;
              }
            }
            else //sentinel encountered
            {
              if(DEBUG){printf("Sentinel %d found at pos %d, stopping.\n",value_read,loc);}
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
        }

        // Initialize rest of memory
        //
        while (loc < MEMLEN)
        {
            loc++;
            cpu->mem[loc] = 0;
        }
}

// Get the data file to initialize memory with.  If it was
// specified on the command line as argv[1], use that file
// otherwise use default.sdc.  If file opening fails, complain
// and terminate program execution with an error.
// See linux command man 3 exit for details.
//
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

// dump_control_unit(CPU *cpu): Print out the control unit
// (PC, IR, running flag, and general-purpose registers).
//
void dump_control_unit(CPU *cpu)
{
  printf("Dumping control unit...\n");
  printf("     PC  %d\n",cpu->pc);
  printf("     IR  %d\n",cpu->ir);
  printf("Running? %d\n",cpu->running);
  dump_registers(cpu);
}

// dump_memory(CPU *cpu): For each memory address that
// contains a non-zero value, print out a line with the
// address, the value as an integer, and the value
// interpreted as an instruction.
//

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
  if((opcode >= 90) && (opcode <= 99)) //use our second nice list...
  {
    return PRINT_INSTRUCTION_NAMES[(opcode - 90)];
  }
  return "WTF";
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


        if(DEBUG == true){
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

        free(info);


        printf("@ %02d   % 05d   % 2s\n",
        loc,
        cpu->mem[loc],
        resolvedOpcode2);
      };
    }

    printf("End of memory dump.\n");

  }

  // *** STUB ****
  // for each location, if the value is nonzero, then
  // print the location and value (as an integer),
  // and call print_instr on the value to print out
  // the value as an instruction
}

// dump_registers(CPU *cpu): Print register values in two rows of
// five.
//
void dump_registers(CPU *cpu)
{
  //init some temp vars to format le code good, so we have:
  //
  // R0: xx R1: xx R2: xx R3: xx R4: xx
  // R5: xx R6: xx R7: xx R8: xx R4: xx
  //

  char *regStr[(7*10)] = {0}; //for spaces and chars blah

  for(int i = 0; i < NREG; i++)
  {
      sprintf((regStr + (7*i)),"R%d: %02d",i,cpu->reg[i]);
  }
    // printf("***We just tried to put \"R%d: %01d\" into partA\n",i,cpu->reg[i]);

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

// print_instr(instr) prints the 4-digit instruction in a mnemonic format.
//
void print_instr(int instr)
{
        // *** STUB ***
}
