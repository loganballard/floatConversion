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

#include <stdio.h>
#include "floatHelpers.h"

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
    float mantissa, result; // the mantissa based on the hex, and the result
    isNeg = calcSign(hex, expTol, fracTol);
    exponent = calcExp(hex, expTol, fracTol, &denorm);
    mantissa = calcFrac(hex, fracTol, denorm);
    if (denorm) {
        if (isSpecialVal(exponent, expTol, mantissa, isNeg)){
            return;
        }
    }
    result = finalFloatCalc(exponent, expTol, mantissa, denorm, isNeg);
    printf("%f\n", result);
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
