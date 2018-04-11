// *** SKELETON -- Replace the STUB comments with real code ****

// CS 350, Fall 2016
// Lab 2 -- Converting a natural number from decimal to a given base
//
// Illinois Institute of Technology, (c) 2016, James Sasaki
//

// Lab completed by Henry Post, hpost@hawk.iit.edu


// This program repeatedly reads in an integer and base
// (both in decimal), converts the integer from decimal
// to the requested base, and prints the result.  We stop
// if the read-in integer value is < 1 or the base is < 2.

// General instructions (for the skeleton):
// Replace all STUB code with actual code.  You don't have
// to use this skeleton, with the warning that this code
// (yours and the skeleton's) are fair game for tests.

#include <stdio.h>
#include <strings.h>
#include <math.h>

// Define array length to be large enough to hold the
// longest possible output (max positive int in base 2).
//
#define ARRAYLEN 32
#define STUDENTNAME "Henry Post"
#define STUDENTEMAIL "hpost@hawk.iit.edu"
#define LEVEL1 "* [01]..."
#define LEVEL2 "*** [02]___"
#define LEVEL3 "***** [03]+++"

int main()
{
	printf("CS 350 Lab 2 for %s, %s\n\n", STUDENTNAME, STUDENTEMAIL);

	// Read an integer value to convert and the base to
	// convert it to
	//
	int value, base;
	printf("Enter an integer and base:\n");
	printf("(int >= 1 and 2 <= base <= 36 or we quit): \n");
	scanf("%d %d", &value, &base);

	// Process the value and base input and prompt for and
	// read in another value and base.  Repeat until the
	// value is < 1 or the base is < 2 or > 36.
	//
	while ( value >= 1 && ( base >= 2 || base <= 36 ))//While valid bounds of input, do:
	{
		// Convert the value into the desired base and put the
		// result into digit[0..column-1].  In general, we've
		// calculated the digits whose powers are less than the
		// current column, with to_convert as the unconverted
		// part of the original value.
		//
		// value == to_convert * (base ^ column) + digits calculated so far
		// with column_value = base ^ column
		//
		int column = 0, to_convert = value;
		int digit[ARRAYLEN];

		/*  5_10 to ?_2...
		//
		//  2^32        etc        2^0
		// [?,    ?,    ?,    ...,  ?]
		//
		// if to_convert >= {base ^ (ARRAYLEN - column)}: we can subtract and "bin" the corresponding value!
		// if to_convert <  {base ^ ()}
		//
		//
		*/


		// *** STUB ***
		while(to_convert != 0)//while we still have a remainder of base-10 numbers
		{
			printf("%sto_convert = %d, column = %d\n",LEVEL1,to_convert,column);

			printf("%sArraylen - column is %d\n",LEVEL1,(ARRAYLEN-column));
			printf("%sbase^(arraylen-col) is %d\n",LEVEL1,(pow(base,(ARRAYLEN-column))));

			printf("%sWill attempt to subtract %d from %d\n",LEVEL1,(pow(base,(ARRAYLEN - column))),(to_convert));
			sleep(3);

			if(to_convert >= (pow(base,(ARRAYLEN - column)))) //if we're good and can subtract
			{
				printf("\n");
				digit[column] = pow(base,(ARRAYLEN - column)) - to_convert; //add the diff. b/w b^n - to_convert

				to_convert = to_convert - pow(base,(ARRAYLEN - column)); //take away what we added
			}
			else
			{ //we're too small, can't convert to the column even partially... must move on!
			//insert sad trumpet here
			printf(" ");
			}
			column += 1;
			printf("\n");
		}
		// *** STUB ***

		if (column >= ARRAYLEN || to_convert < 0 || to_convert > 0)
		{
			printf("Ran out of room or to_convert is < 0");
			printf("Or to_convert is not zero, meaning that we may have missed some values!");
			printf(" -- This should never happen!\n");
		}
		else
		{
			// Print out the digits (note they're stored in
			// reverse: digit[0] is the rightmost digit,
			// ..., digit[column-1] is the leftmost digit
			//
			char prt_digits[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			// *** STUB ***
		}
		printf("\n");

		// Get next value, base pair to process
		//

		// *** STUB ***
	}



}
