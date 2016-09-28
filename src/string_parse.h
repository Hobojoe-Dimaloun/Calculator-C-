#ifndef STRING_PARSE_H_
#define STRING_PARSE_H_

#include <stdio.h>

int functionLetterRead(char **inputString, char **stack,int *stackcounter );

double numberRead(char **inputString);

double constantsLetterRead(char **inputString);

void symbolRead(char **inputString, char **stack, char *functionOutput[],int *stack_Counter,int *function_Output_Counter);


#endif // STRING_PARSE_H_
