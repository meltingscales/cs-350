// Lab completed by Henry Post, hpost@hawk.iit.edu
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define STUDENTNAME "Henry Post"
#define STUDENTEMAIL "hpost@hawk.iit.edu"
#define LEVEL1 "**[L01]**"
#define LEVEL2 "****[L02]****"
#define LEVEL3 "*****[L03]*****"
#define DICT "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

int logicRShift(int number, int places)
{ //zero fill all the time
  return (unsigned)number >> places;
}

int signExtension(int number, int places) //this does...something
{
  if(((number >> (places-1)) & 1 ) == 1) //if msb == 1
  {
    number = (-1 << places | number); //tack on the places we're using
  }
  return number; //return pos unsigned if passed pos, return neg signed if passed neg number
}

int main(int argc, char *argv[])
{

printf("we got passed %d arguments! woo!!!!\n",argc);

  char *filename;
  FILE *in_file;

  printf("%s, %s\n",STUDENTNAME,STUDENTEMAIL);

  if(argv[1] != NULL){filename = argv[1];}
  else{filename="Lab3_data.txt";}

  printf("I've been passed %s as an argument.\n",filename);



  in_file = fopen(filename, "r");

  if(in_file == NULL)
  {
    printf("Our file is nonexistant or we cannot access it!\n");
    return 1;
  }
  else
  {
    printf("Successfully opened %s\n\n",filename);
  }

  int val, len1, len2, len3;
  int nbr_vals_on_line = fscanf(in_file, "%x %d %d", &val, &len1, &len2);

  while(nbr_vals_on_line == 3) //break up val into bitstrings
  {
    len3 = 32 - (len1 + len2);


    if((len1 <= 0) || (len2 <= 0) || (len3 <= 0))
    {
      printf("%d, %d, and %d are not all positive. Please fix.\n",len1,len2,len3);
    }
    else
    {
      int x1, x2, x3;

      x1 = ((logicRShift(val,(len3 + len2)))); //unsigned, first "len1" bits

      x2 = (val << (len1)); //chop off first part
      x2 = ((logicRShift(x2,(len3 + len1)))); //chop off 2nd part


      x3 = (val << (len1 + len2));// chop off first part
      x3 = (logicRShift(x3,(len1 + len2))); //shift back to remove the zeroes


      printf("Value = %#08x, decimal\n",val);
      printf("Its leftmost  %2d bits are %#2x = %2d as an unsigned integer\n",	len1, x1, x1 );



      int x2b;

      if((signExtension(x2,len2) < 0)) //if MSB is 1, meaning it's negative
      {
        x2b = x2 + 1;
        x2b = (x2b << (32-len2));     //these take care of negative zeroes...
        x2b = (logicRShift(x2b,(32-len2)));
      }
      else
      { //it's pos, don't convert
        x2b = x2;
      }


      printf("Its next      %2d bits are %#2x = %2d as a 1's complement integer\n", len2, x2, signExtension(x2b,len2));

      printf("Its remaining %2d bits are %#2x = %2d as a 2's complement integer\n", len3, x3, signExtension(x3,len3));

      // *** STUB *** print len3, x3, and decimal value of x3 in 2's comp
      //calc x3 as a 2's complement integer
    }

    printf("\n");
    nbr_vals_on_line = fscanf(in_file, "%x %d %d", &val, &len1, &len2);
  }
  return 0;
}
