//Henry Post, HenryFBP@gmail.com, hpost@hawk.iit.edu
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

int *toBaseN(int, int);
char *toBaseNString(int, int);

int signExtension(int number, int places) //this
{
  if(((number >> (places-1)) & 1 ) == 1) //if msb == 1
  {
    number = (-1 << places | number); //tack on the places we're using
  }
  return number; //return the now-negative number..?
}

int logicRShift(int number, int places)
{
  return (unsigned)number >> places;
}

int *toBaseN(int number, int base)
{
  if(number >=1 && (base >=2 && base <=36))//while within valid bounds of val,base
  {
    int converted_len, column, to_convert;
    column = 0;
    to_convert = number;

    while (pow(base,column) <= to_convert) //while we're too small or equal
    {
      column++; //inch up column to max arr len
    }

    int* ret;
    ret = malloc(column);

    int converted_arr[column]; //make empty arr for vals
    initIntArr(converted_arr,column);


    for (int i = column; i >= 0; i=i-1)//loop through all columns, i.e. 16^2, then 16^1, etc
    {
      while(to_convert >= ((int)(pow(base,i))) )//while we can subtract 16^x from to_convert
      {
        to_convert = to_convert - ((int)(pow(base,i))); //take away what we added to the b^i's place
        converted_arr[i]++; //add a "ones" or "16s" or "256s" place, depending on what 16^i is.
      }
    }



    for (int i = column; i > 0; i--) //our list is backwards, and we need to convert.
    {
      ret[column-i] = converted_arr[i-1];//converted string[col-i] = dict[number-1]
    }
    //no idea why i minus one, but whatever hatches your turnips.

  return ret;
  }
}

void writeFile(char filePath[], char string[])
{
  FILE *f;
  f = fopen(filePath, "w");

  if(f == NULL){exit(-1);printf("bad time opening file. bad. oops.\n");}

  fprintf(f, "%s", string);
  fclose(f);
}

char *readFile(char filePath[])
{
  FILE *f;
  f = open(filePath, "r");

  if(f == NULL){exit(-1);printf("bad time opening file. bad. oops.\n");}

  char *fileData = 'd';
  return 'd';
  fclose(f);
}

int intpow(int x, int y)
{
  return ((int)(pow(x,y)));
}

void printIntArr(int arr[], int len)
{
  for (int i = 0; i < len; i++)
  {
    printf("[PrntArr]%i\n",arr[i]);
  }
}

void initIntArr(int arr[], int len)
{
  for (int i = 0; i < len; i++)
  {
    arr[i] = 0;
  }
}

void initCharArr(char arr[], int len)
{
  for (int i = 0; i < len; i++)
  {
    arr[i] = 0;
  }
}
