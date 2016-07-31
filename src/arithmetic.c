#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
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

void mathematicalOperation(char functionOutput[],double numberOutput[], int *number_Output_Counter, int *function_Output_Counter);

int symbolRead(char **inputString, char stack[], char functionOutput[], int *stack_Counter ,int *function_Output_Counter);


int arithmetic(char *inputString)
{
    int functionOutputCounter=0;
    int stackCounter=0;
    int numberOutputCounter=0;
    int *number_Output_Counter=&numberOutputCounter, *function_Output_Counter=&functionOutputCounter, *stack_Counter=&stackCounter;
    char stack[10], functionOutput[100];
    double numberOutput[100];


    while(*inputString!='\n' && *inputString!='\0')
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
                else if(-1==bannedLetterRead())
                {
                    break;
                }

        }
        else
        {
            if(*inputString=='-' && (!isdigit(*(inputString-1)) && !isalpha(*(inputString-1))))
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

    int i;

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
    return(0);
}


void mathematicalOperation(char functionOutput[],double numberOutput[],int *number_Output_Counter, int *function_Output_Counter)
{
    double working[2];
    // Selects last number to be entered
    (*number_Output_Counter)--;
    //Performs Unary minus, requires only one working number.
    if(functionOutput[*function_Output_Counter]=='#')
    {
        working[1]=numberOutput[*number_Output_Counter];
        numberOutput[*number_Output_Counter]=working[1]*(-1);
    }
    else
    {
        //Pops two numbers off the output stack to the working array

        working[1]=numberOutput[*number_Output_Counter];
        (*number_Output_Counter)--;
        working[0]=numberOutput[*number_Output_Counter];

        //Calculates the operation and pushes the result onto the output stack
        switch(functionOutput[*function_Output_Counter])
        {
        case '+':{
                    numberOutput[*number_Output_Counter]=working[0]+working[1];
                    break;
                }
        case '-':{
                    numberOutput[*number_Output_Counter]=working[0]-working[1];
                    break;
                }
        case '*':{
                    numberOutput[*number_Output_Counter]=working[0]*working[1];
                    break;
                }
        case '/':{
                    numberOutput[*number_Output_Counter]=working[0]/working[1];
                    break;
                }
        case '^':{
                    numberOutput[*number_Output_Counter]=pow(working[0],working[1]);
                    break;
                }
        }
    }
    // Returns the output stack position to the next empty cell.
    (*number_Output_Counter)++;
}

int symbolRead(char **inputString, char stack[], char functionOutput[],int *stack_Counter,int *function_Output_Counter)
{
    char *inputLocal=NULL;
    inputLocal=*inputString;
    switch(*inputLocal)
    {
        case '(': stack[*stack_Counter]=*inputLocal; (*stack_Counter)++; break;       //Assign input and move to new empty input

        case ')':{  if(*stack_Counter>0)                                                  //Prevents stack underflow
                    {
                        (*stack_Counter)--;                                                 //Return counter to the last input
                        while(stack[*stack_Counter]!='(')
                        {
                            functionOutput[*function_Output_Counter]=stack[*stack_Counter];                  //Pop top input onto output stack and advance output stack
                            (*function_Output_Counter)++;
                            if(*stack_Counter>=0)                                         //Prevents underflow of stack
                            {
                                (*stack_Counter)--;                                         //Current input has been popped, therefore advance back one step to previous input
                            }
                            else{ printf("Non-Paired brackets.\n");break;}
                        }

                        break;                                                          //If '(' is encountered the stack counter does not move. Allows overwrite of '('
                    }
                    else{stack[*stack_Counter]=*inputLocal; (*stack_Counter)++; break;}
                }


        case '^':stack[*stack_Counter]=*inputLocal; (*stack_Counter)++; break;       //Assign input and move to new empty input

        case '/':
        case '*':{
                    if(*stack_Counter>0)                                                  //Prevents stack underflow
                    {
                        (*stack_Counter)--;                                                 //Return counter to the last input
                        switch(stack[*stack_Counter])
                        {
                            case '-':
                            case '+':{
                                        (*stack_Counter)++;                                     //Advance Counter to empty cell
                                        stack[*stack_Counter]=*inputLocal;            //Input into cell and advance
                                        (*stack_Counter)++;
                                        break;
                                     }
                            case '*':
                            case '/':
                            case '^':
                            case '#':{                                                       //equal precedence and left associative
                                        functionOutput[*function_Output_Counter]=stack[*stack_Counter];          //Pop '/' to output and advance output counter
                                        (*function_Output_Counter)++;
                                        stack[*stack_Counter]=*inputLocal;            //Push '/' onto stack and advance stack Counter
                                        (*stack_Counter)++;
                                        break;
                                     }
                            default : stack[*stack_Counter]=*inputLocal; (*stack_Counter)++; break;
                            }

                        }
                        else{stack[*stack_Counter]=*inputLocal; (*stack_Counter)++; break;}
                        break;
                    }
        case '+':
        case '-': {
                    if(*stack_Counter>0)                                                  //Prevents stack underflow
                    {
                        (*stack_Counter)--;                                                 //Return counter to the last input
                        switch(stack[*stack_Counter])
                        {
                            case '-':
                            case '+':
                            case '*':
                            case '/':
                            case '^':
                            case '#':{
                                        functionOutput[*function_Output_Counter]=stack[*stack_Counter];          //Pop '*' to output and advance output counter
                                        (*function_Output_Counter)++;
                                        stack[*stack_Counter]=*inputLocal;                    //Push '/' onto stack and advance stack Counter
                                        (*stack_Counter)++;
                                        break;
                                    }
                            default : (*stack_Counter)++;stack[*stack_Counter]=*inputLocal; (*stack_Counter)++; break;
                        }

                    }
                    else{stack[*stack_Counter]=*inputLocal; (*stack_Counter)++; break;}
                    break;
                    }
        case '#':{
                    stack[*stack_Counter]=*inputLocal;                    //Push '#' onto stack and advance stack Counter
                    (*stack_Counter)++;
                }
        default : break;


        }
    inputLocal++;
    *inputString=inputLocal;                                                             // advances the input string pointer to the next character

    return(0);
}

