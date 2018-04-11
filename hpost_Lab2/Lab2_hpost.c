// Lab completed by Henry Post, hpost@hawk.iit.edu


#include <stdio.h>
#include <strings.h>
#include <math.h>
#include <stdbool.h>
#include "../henryfunc.c"

#define DEBUG true
#define DICT "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

// DICT[15] = 'F';

int main()
{

	printf("CS 350 Lab 2 for %s, %s\n\n", STUDENTNAME , STUDENTEMAIL);

	int value, base;



	void ask()
	{
		printf("Enter an integer and base:\n");
		printf("(int >= 1 and 2 <= base <= 36 or we quit): ");
		scanf("%d %d", &value, &base);
	}


	ask();
	

	while(value >= 1 && (base >=2 && base <=36))//while within valid bounds of val,base
	{
		int converted_len, column, to_convert;
		column = 0;
		to_convert = value;

		while (pow(base,column) <= to_convert) //while we're too small or equal
		{
			column++; //inch up column to max arr len
		}

		if(DEBUG == true){printf("%smax column is %d, or %d^%d or %d with value %d\n",LEVEL1,column,base,column,(int)(pow(base,column)),to_convert);}

		int converted_arr[column]; //make empty arr for vals
		initIntArr(converted_arr,column);

		for (int i = column; i >= 0; i--)//loop through all columns, i.e. 16^2, then 16^1, etc
		{
			if(DEBUG == true){printf("%sat col %d^%d\n",LEVEL2,base,i);}

			while(to_convert >= ((int)(pow(base,i))))//while we can subtract 16^x from to_convert
			{
				if(DEBUG == true){printf("%sAbout to %d - %d \n",LEVEL3,to_convert,((int)(pow(base,i))));}

				to_convert = to_convert - ((int)(pow(base,i))); //take away what we added to the b^i's place
				converted_arr[i]++; //add a "ones" or "16s" or "256s" place

				if(DEBUG == true){printf("%sAdded one to %d^%d, left with %d\n",LEVEL3,base,i,to_convert);}
				if(DEBUG == true){printf("%sarr[%d]=%d\n",LEVEL3,i,converted_arr[i]);}
			}
		}

		//below is base10-basewhatevs


		if(DEBUG == true){printIntArr(converted_arr,column);}

		char converted_string[column+1]; //got darn '\0' character...
		initCharArr(converted_string,column+1); //yup

		for (int i = column; i > 0; i--) //our list is backwards, and we need to convert.
		{
			if(DEBUG == true){printf("%sAt i=%d\n",LEVEL3,i);}
			converted_string[column-i] = DICT[converted_arr[i-1]];//converted string[col-i] = dict[number-1]
		}
		printf("%s.\n\n", converted_string);

		ask();
	}

	//145 to base-16?
	//
	//16^0 = 1,  small.
	//16^1 = 16, small.
	//16^2 = 256, big.
	//
	//145-16=129 ; 1x
	//129-16=113 ; 2x
	//113-16=97  ; 3x
	//97 -16=81  ; 4x
	//81 -16=65  ; 5x
	//65 -16=49  ; 6x
	//49 -16=33  ; 7x
	//33 -16=17  ; 8x
	//17 -16=1   ; 9x
	//1-16...too small
	//1  -1 =0   ; 91 in hex!
}
