/*
 *  Logan Ballard
 *  Assignment 2 - Floating Point Number Parser
 *
 *  This project will take several inputs and together with some
 *  mathemagical logic, will create a floating point representation
 *  of a hexidecimal number.
 *
 *  It will read in a few parameters, check them for validity, then
 *  using properties of floating point numbers, it will approximate
 *  to the best of its ability the floating point number that 
 *  characterizes the hexidecimal number passed in, based on the 
 *  (also passed in) restrictions for the exponent and fraction it 
 *  can use.
 *
 *  This will work with both normalized and denormalized floating
 *  point representations.
 *
 *  (c) Logan Ballard, 2018
*/

// includes
#include <stdio.h>

// definitions
#define DEBUG 1

/*
 * processInput
 *  This function will take the input from the command line
 *  and process it into the correct format, printing out errors
 *  as it finds them.
 *
 *  Params:
 *      int argCount        - the number of arguments passed in
 *      char *argValues[]   - the arguments
 *      int *frac           - pointer to fraction value restriction
 *      int *exp            - pointer to exp value restriction
 *      unsigned int *hex   - pointer to the hex number to convert
 *
 *  Returns:
 *      0 - successful processing of input (no errors)
 *      1 - unsuccessful processing of input (at least 1 error)
*/
int processInput(int argCount, char *argValues[], 
        int *frac, int *exp, unsigned int *hex) { 
    if (argCount != 4) {
         printf("Wrong number of arguments! Expected (3),");
         printf("received (%d).\n",argCount);
         return 1;
    }
    sscanf(argValues[1],"%d",frac); // fraction allowed range
    sscanf(argValues[2],"%d",exp); // exp allowed range
    sscanf(argValues[3],"%x",hex); // hex number to encode to float
    if (*frac < 2 || *frac > 10 ) {
        printf("Illegal number of fraction bits (%d), ",*frac);
        printf("should be between 2 and 10.\n");
        return 1;
    } else if (*exp < 3 || *exp > 5) {
        printf("Illegal number of exp bits (%d), ",*exp);
        printf("should be between 3 and 5.\n");
    } else {
        return 0;
    }
}


int main(int argc, char *argv[]) {
    int exp, frac; // exponent, fraction tolerance
    unsigned int hex; // hexidecimal number to encode
    if (processInput(argc, argv, &frac, &exp, &hex)) {
        return 1; // something went wrong
    }
    return 0;
}
