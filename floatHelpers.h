// helper function stubs

int processInput(int argCount, char *argValues[], int *frac, int *exp, unsigned int *hex);
int calcBias(int expTol);
int calcSign(unsigned int hex, int expTol, int fracTol);
int calcExp(unsigned int hex, int expTol, int fracTol, int *denorm);
float calcFrac(unsigned int hex, int fracTol, int denorm);
int isSpecialVal(int exp, int expTol, float frac, int isNeg);
float finalFloatCalc(int exp, int expTol, float mantissa, int denorm, int isNeg);
