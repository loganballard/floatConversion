CC=gcc

assign2: floatConversion.c floatHelpers.c floatHelpers.h
	    $(CC) -o floatConversion floatConversion.c floatHelpers.c -lm

