// *** SKELETON -- Replace the STUB comments with real code ****
// This skeleton doesn't compile as is.

// CS 350, Fall 2016
// Lab 3 -- Breaking up a bitstring
//
// Illinois Institute of Technology, (c) 2016, James Sasaki
//

// This program processes each line of a file; the filename is
// specified as a command-line argument (argv[1]) or defaults to
// Lab3_data.txt. Each line should contain a hex integer value and
// two decimal integers, len1 and len2. We find the leftmost len1
// bits of the value, the next len2 bits of the value, and the
// rightmost len3 = (32 - (len1 + len2)) remaining bits. (All 3
// values should be > 0 else we signal an error and skip to the
// next line of input.)  We print the three bitstrings in hex and
// in decimal. The leftmost string is unsigned, the middle string
// is in 1's complement, and the rightmost string is in 2's
// complement.

// General instructions (for the skeleton):
// Replace all STUB code with actual code.  You don't have
// to use this skeleton, with the warning that this code
// (yours and the skeleton's) are fair game for tests.

#include <stdio.h>

int main (int argc, char *argv[]) {

	// Get the filename to open, then open the file. If we can't
	// open the file, complain and quit.
	//
	char *filename;
	// *** STUB *** if argv[1] exists, set filename to it,
	// otherwise set filename to "Lab3_data.txt";
	
	// Open the file; if opening fails, say so and return 1.
	// otherwise say what file we're reading from.
	FILE *in_file;
	in_file = fopen(filename, "r");		// NULL if the open failed
	// *** STUB *** if in_file is NULL, complain and quit
	// otherwise say that we've opened the filename

	// Repeatedly read and process each line of the file.  The
	// line should have a hex integer and two integer lengths.
	//
	int val, len1, len2, len3;
	int nbr_vals_on_line
		= fscanf(in_file, "%x %d %d", &val, &len1, &len2);

	// Read until we hit end-of-file or a line without the 3 values.
	while (nbr_vals_on_line == 3) {
		// We're going to break up the value into bitstrings of
		// length len1, len2, and len3 (going left-to-right).
		// The user gives us len1 and len2, and we calculate
		// len3 = the remainder of the 32 bits of value.
		// All three lengths should be > 0, else we complain
		// and go onto the next line.
		//
		len3 = 32 - (len1 + len2);

		// *** STUB ***
		// if any of the lengths aren't > 0,
		//     print out the value and the lengths and complain
		//     about the lengths not all being positive
		else {
			// Calculate the 3 bitstrings x1, x2, x3 of lengths
			// len1, len2, and len3 (reading left-to-right).
			//
			// *** STUB *** 
		
			// Calculate the value of x2 read as a 1's complement int.
			// Calculate the value of x3 read as a 2's complement int.
			//
			// *** STUB *** 

			// Print out the original value as a hex string, print out
			// (going left-to-right) each length (in decimal) and selected
			// bitstring (in hex), and its decimal value.  We read x1 as
			// unsigned, x2 in 1's complement, and x3 in 2's complement.
			//
			printf("Value = %#08x\n", val);
			printf("Its leftmost  %2d bits are %#x = %d as an unsigned integer\n",
				len1, x1, x1 );
			// *** STUB *** print len2, x2, and decimal value of x2 in 1's comp
			// *** STUB *** print len3, x3, and decimal value of x3 in 2's comp
		}
		printf("\n");
		nbr_vals_on_line = fscanf(in_file, "%x %d %d", &val, &len1, &len2);
	}
	return 0;
}
