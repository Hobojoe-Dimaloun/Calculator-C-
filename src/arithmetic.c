#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "string_parse.h"

/* ****************
*   BIDMAS Precedence
*
*   Unary-0
*   Brackets-1
*   Indices-2
*   Division-3
*   Multiplication-3
*   Addition-4
*   Subtraction-4
**********************/

void mathematicalOperation(char *functionOutput[],double numberOutput[], int *number_Output_Counter, int *function_Output_Counter);


int arithmetic(char *inputString)
{
    int i;
    int functionOutputCounter=0;
    int stackCounter=0;
    int numberOutputCounter=0;
    int *number_Output_Counter=&numberOutputCounter, *function_Output_Counter=&functionOutputCounter, *stack_Counter=&stackCounter;
    char **stack=NULL, *functionOutput[100];
    double numberOutput[100];
    bool first=true;

    if((stack=malloc(10*sizeof(char*)))==NULL)
    {
        printf("Error - memory not assigned");
        return(-1);
    }
    for(i=0;i<10;i++)
    {
        stack[i]=NULL;

    }


    while(*inputString!='\0')
    {
        // Sorts the characters into reverse polish notation.
        // Digits and symbols are separated into two output stacks.
        if(isdigit(*inputString))
        {
            numberOutput[numberOutputCounter]=numberRead(&inputString);
            numberOutputCounter++;
        }
        else if(isalpha(*inputString))
        {
                if((numberOutput[numberOutputCounter]=constantsLetterRead(&inputString))!=0)
                {
                    numberOutputCounter++;
                }
                else if(functionLetterRead(&inputString,stack,stack_Counter)==0)
                {

                }
                else
                {
                    break;
                }

        }
        else
        {
            if(*inputString=='-' && (first==true || (!isdigit(*(inputString-1)) && !isalpha(*(inputString-1)))))
            {
                *inputString='#';
            }
            symbolRead(&inputString,stack,functionOutput,stack_Counter,function_Output_Counter);

        }

        // Functions on the output cue are computed 'on the fly'.
        while(functionOutputCounter>0)
        {

            functionOutputCounter--;
            mathematicalOperation(functionOutput,numberOutput,number_Output_Counter,function_Output_Counter);

        }
        first=false;
    }
    // Once entire equation has been parsed, remaining
    // functions are added to the end of the output
    // from the stack as required via shunting yard algorithm.
    stackCounter--;
    for(stackCounter;stackCounter>=0;stackCounter--)
    {
        functionOutput[functionOutputCounter]=stack[stackCounter];
        functionOutputCounter++;
    }
    int hold;
    hold=functionOutputCounter;
    functionOutputCounter=0;

    // Final computation occurs using reverse polish notation.
    while(hold>0 )
        {
            hold--;

            mathematicalOperation(functionOutput,numberOutput,number_Output_Counter,function_Output_Counter);

            functionOutputCounter++;
        }



    // Large numbers are displayed using standard form,
    // small numbers are displayed in float form.

    for(i=0;i<numberOutputCounter;i++)
    {
        if(numberOutput[i]>=1E7)
        {
            printf("%E\n",numberOutput[i]);
        }
        else
        {
            printf("%lf\n",numberOutput[i]);
        }

    }
    for(i=0;i<10;i++)
    {
        if(stack[i]!=NULL)
        {
            free(stack[i]);
        }

    }
     free(stack);
    return(0);
}


void mathematicalOperation(char *functionOutput[],double numberOutput[],int *number_Output_Counter, int *function_Output_Counter)
{
    double working[2];
    // Selects last number to be entered
    (*number_Output_Counter)--;
    //Performs Unary minus, requires only one working number.
    working[1]=numberOutput[*number_Output_Counter];
    if(0==strncmp(functionOutput[*function_Output_Counter],"#",1))
    {
        numberOutput[*number_Output_Counter]=working[1]*(-1);
    }
    else if(0==strcmp(functionOutput[*function_Output_Counter],"sin"))
    {
        numberOutput[*number_Output_Counter]=sin(working[1]);
    }
    else if(0==strcmp(functionOutput[*function_Output_Counter],"cos"))
    {
        numberOutput[*number_Output_Counter]=cos(working[1]);
    }
    else if(0==strcmp(functionOutput[*function_Output_Counter],"tan"))
    {
        numberOutput[*number_Output_Counter]=tan(working[1]);
    }
    else if(0==strcmp(functionOutput[*function_Output_Counter],"sinh"))
    {
        numberOutput[*number_Output_Counter]=sinh(working[1]);
    }
    else if(0==strcmp(functionOutput[*function_Output_Counter],"cosh"))
    {
        numberOutput[*number_Output_Counter]=cosh(working[1]);
    }
    else if(0==strcmp(functionOutput[*function_Output_Counter],"tanh"))
    {
        numberOutput[*number_Output_Counter]=tanh(working[1]);
    }
    else if(0==strcmp(functionOutput[*function_Output_Counter],"ln"))
    {
        numberOutput[*number_Output_Counter]=log(working[1]);
    }
     else if(0==strcmp(functionOutput[*function_Output_Counter],"exp"))
    {
        numberOutput[*number_Output_Counter]=exp(working[1]);
    }
    else if(0==strcmp(functionOutput[*function_Output_Counter],"log"))
    {
        numberOutput[*number_Output_Counter]=log10(working[1]);
    }
     else if(0==strcmp(functionOutput[*function_Output_Counter],"sqrt"))
    {
        if(working[1]>0)
        {
            numberOutput[*number_Output_Counter]=sqrt(working[1]);
        }
    }
    else if(*number_Output_Counter>=1)
    {
        //Pops two numbers off the output stack to the working array

        (*number_Output_Counter)--;
        working[0]=numberOutput[*number_Output_Counter];

        //Calculates the operation and pushes the result onto the output stack
        if(0==strcmp(functionOutput[*function_Output_Counter],"+"))
        {
            numberOutput[*number_Output_Counter]=working[0]+working[1];
        }
        else if(0==strcmp(functionOutput[*function_Output_Counter],"-"))
        {
            numberOutput[*number_Output_Counter]=working[0]-working[1];
        }
        else if(0==strcmp(functionOutput[*function_Output_Counter],"*"))
        {
            numberOutput[*number_Output_Counter]=working[0]*working[1];
        }
        else if(0==strcmp(functionOutput[*function_Output_Counter],"/"))
        {
            numberOutput[*number_Output_Counter]=working[0]/working[1];
        }
        else if(0==strcmp(functionOutput[*function_Output_Counter],"^"))
        {
            numberOutput[*number_Output_Counter]=pow(working[0],working[1]);
        }
    }

    // Returns the output stack position to the next empty cell.
    (*number_Output_Counter)++;
}


