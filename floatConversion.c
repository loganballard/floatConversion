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
#include <math.h>

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
         printf("received (%d).\n",(argCount-1));
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
        return 1;
    } else {
        return 0;
    }
}

/*
 * calcBias
 *  Programmatically calculates the bias 
 *
 *  Params:
 *      int expTol - the amount of exp digits
 *
 *  Returns:
 *      int - the bias that we will use in calculations
*/
int calcBias(int expTol) {
    return (pow(2,expTol - 1) - 1);
}

/*
 * calcSign
 *  This calculates the sign (-/+), based on the hex value
 *  given, and the fractional and exponent tolerance
 *
 *  Params:
 *      unsigned int hex - the user inputted hex value
 *      int exp          - the exponential tolerance
 *      int frac         - the fractional tolerance
 *
 *  Returns:
 *      1 - the parsed float will be negative
 *      0 - the parsed float will be positive
*/
int calcSign(unsigned int hex, int exp, int frac) {
    hex = hex >> (exp + frac);
    return (hex & 1);
}

/*
 * calcExp
 *  Function to calculate the value of the exponential portion 
 *  of a hex value.  Does so via bit shifting and bit masking
 *
 *  Params:
 *      unsigned int hex - the hex value to get the exp from
 *      int exp          - the exponent tolerance
 *      int frac         - the fractional tolerance
 *      int *denorm      - whether to use the normal or denorm
 *                         range in calculation. Passed by ref
 *                         from caller and set here in calcExp
 *  Returns:
 *      int - the value that was calculated for the exponent
 *            portion of the hex value
*/
int calcExp(unsigned int hex, int exp, int frac, int *denorm) {
    unsigned int expBitMask = (1 << exp) - 1; // create mask
    int retExp;
    hex = hex >> frac;
    retExp = hex & expBitMask;
    if (retExp == 0 || retExp == expBitMask) {
        *denorm = 1;
    } else {
        *denorm = 0;
    }
    return retExp;
}

/*
 * calcFrac
 *  Calculates the fractional portion of the float representation.
 *  Is dependent on normalized flag being set accurately by caller. 
 *  As a consequence, must be called after calcExp.
 *
 *  Params:
 *      unsigned int hex - the hex value to convert
 *      int fracTol      - fractional tolerance
 *      int denorm       - 1 to use denorm range, 0 for normal range
 *
 *  Returns:
 *      float - fractional portion of float representation of hex
*/
float calcFrac(unsigned int hex, int frac, int denorm) {
    int fracBitMask = (1 << frac) - 1;
    int fracVal = hex & fracBitMask;
    float fracRet = 0;
    for (int i = 0; i < frac; i++) {
        if (fracVal & (1 << i)) {
            fracRet += pow(2,-(frac-i));
        }
    }
    if (!denorm) {
        return 1 + fracRet;
    } else {
        return fracRet;
    }
}

/*
 * isSpecialVal
 *  Handing the situation in the denormalized range where
 *  the exponent's bits are all 1
 *
 *  Params:
 *      int exp    - the exponent value to check if all bits are 1
 *      int expTol - # of bits in the exponent
 *      float frac - the fractional value
 *      int isNeg  - negative or positive (1 = negative)
 *
 *  Returns:
 *      1 - special value was handled, caller should stop calculations
 *      0 - no special value found, caller should continue
*/
int isSpecialVal(int exp, int expTol, float frac, int isNeg) {
    // check for exponents bits all = 1
    if ( !( exp == (exp & ((1 << expTol) - 1)) )) {
        return 0; // not a special value, return
    }
    if (frac == 0) {
        if (isNeg) {
            printf("-inf\n");
        } else {
            printf("inf\n");
        }
    } else {
        printf("NaN\n");
    }
    return 1;
}

/*
 * calcFloat
 *  Does the main work for this program. Figures out the
 *  float representation of the hex number inputted by the 
 *  user, and returns it based on some constraints also
 *  put in by the user.  Prints out the float. No return value
 *
 *  Params:
 *      int fracTol      - fractional tolerance (# of bits that
 *                         are used to calculate the fraction)
 *      int expTol       - same idea, but the exponent tolerance
 *      unsigned int hex - the hex value the user input
 *
 *  Returns:
 *      Nothing
*/      
void calcFloat(int fracTol, int expTol, unsigned int hex) {
    int isNeg, exponent; // the sign (neg/pos), exponent val from hex
    int denorm; // flag to use the normalized/denormalized range 
    float fraction, result; // the frac based on the hex, and the result
    isNeg = calcSign(hex, expTol, fracTol);
    exponent = calcExp(hex, expTol, fracTol, &denorm);
    fraction = calcFrac(hex, fracTol, denorm);
    if (denorm) {
        if (isSpecialVal(exponent, expTol, fraction, isNeg)){
            return;
        }
    }
    //result = pow((1 + frac),2^(exponent - calcBias(expTol)));
    //printf("%f\n", result);
    return;
}

int main(int argc, char *argv[]) {
    int exp, frac; // exponent and fraction tolerance, bias
    unsigned int hex; // hexidecimal number to encode
    if (processInput(argc, argv, &frac, &exp, &hex)) {
        return 1; // something went wrong
    }
    calcFloat(frac, exp, hex);
    return 0;
}
