#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "string_parse.h"

 enum mathFunctions
{
    SIN,COS,TAN,SINH,
    COSH,TANH,ARCSIN,ARCCOS,
    ARCTAN,ARCSINH,ARCCOSH,ARCTANH,
    LN,EXP,LOG,
    SQRT,FINAL_MATH_PLACE_HOLDER
};

static const char *mathLookUp[]=
{
        "sin","cos","tan","sinh",
        "cosh","tanh","arcsin","arccos",
        "arctan","arcsinh","arccosh","arctanh",
        "ln","exp","log","sqrt"
};

double numberRead(char **inputString)
{
    double numberOutput;
    char *inputLocal=NULL;
    inputLocal=*inputString;

    // Converts the string number to a double and moves
    // the string position to the next character.
    numberOutput=strtod(inputLocal,&inputLocal);

    *inputString=inputLocal;

    return(numberOutput);
}

double constantsLetterRead(char **inputString)
{
    const double e=2.718281828;
    const double PI=3.141592654;
    double numberOutput;
    char *inputLocal=NULL;
    inputLocal=*inputString;

    // If PI or Euler's number is entered as a symbol, the corresponding digits are
    // put in their place. Other letters cause a failure and program exit.
    switch(*inputLocal)
    {
        case 'P':
        case 'p': if(*(inputLocal+1)=='i' || *(inputLocal+1)=='I'){inputLocal+=2;numberOutput=PI; break;} else{return(1); break;}
        case 'e': inputLocal++;numberOutput=e; break;
        default: return(0); break;
    }

    *inputString=inputLocal;
    return(numberOutput);
}

int functionLetterRead(char **inputString, char **stack,int *stackcounter )
{
    char *inputLocal=NULL,*temp;
    inputLocal=*inputString;
    int i=0,j,loop=0;

    if((temp=malloc(8*sizeof(char)))==NULL)
    {
        printf("Error - memory not assigned");
        return(-1);
    }
    memset(temp,'\0',8);

    // copy string to temp location
    strncpy(temp,inputLocal,7);

    // Cycle through the string from the REAR.
    // If no string match reduce the length of the string by 1.
    // If string match occurs, push string onto stack.
    // If no match, return -1 for error handling
    while(loop==0)
    {

        for(j=0;j<FINAL_MATH_PLACE_HOLDER;j++)
        {
            if(strncmp(mathLookUp[j],temp,7)==0)
            {
                stack[*stackcounter]=temp;
                (*stackcounter)++;
                inputLocal+=strlen(temp);
                loop=1;
                break;
            }
        }
        if(i<6 && loop!=1)
        {
            temp[8-i]='\0';
            i++;
        }
        else if(i>=6 && loop!=1)
        {
            // return failure. Is not a trig function
            return(-1);
        }
    }
    *inputString=inputLocal;
    return(0);
}

int bannedLetterRead()
{
    // If this is used then the letter is 'banned' and -1 returned

    return(-1);
}

void symbolRead(char **inputString, char **stack, char *functionOutput[],int *stack_Counter,int *function_Output_Counter)
{
    char *inputLocal=NULL, *temp;
    temp=malloc(5*sizeof(char));
    memset(temp,'\0',5);
    inputLocal=*inputString;
    temp[0]=*inputLocal;


        switch(*inputLocal)
        {
            case '(': stack[*stack_Counter]=temp; (*stack_Counter)++; break;       //Assign input and move to new empty input

            case ')':{  if(*stack_Counter>0)                                                  //Prevents stack underflow
                        {
                            (*stack_Counter)--;                                                 //Return counter to the last input
                            while(*stack[*stack_Counter]!='(')
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
                        else{stack[*stack_Counter]=temp; (*stack_Counter)++; break;}
                    }


            case '^':    if(*stack_Counter>0 && (*stack[*stack_Counter-1]=='^' || isalpha(*stack[*stack_Counter-1])))
                        {
                            (*stack_Counter)--;
                            functionOutput[*function_Output_Counter]=stack[*stack_Counter];          //Pop '/' to output and advance output counter
                            (*function_Output_Counter)++;
                            stack[*stack_Counter]=temp;            //Push '/' onto stack and advance stack Counter
                            (*stack_Counter)++;
                            break;
                        }
                        else
                        {
                            stack[*stack_Counter]=temp; (*stack_Counter)++; break;       //Assign input and move to new empty input
                        }

            case '/':
            case '*':{
                        if(*stack_Counter>0)                                                  //Prevents stack underflow
                        {
                            (*stack_Counter)--;                                                 //Return counter to the last input
                            switch(*stack[*stack_Counter])
                            {
                                case '-':
                                case '+':{
                                            (*stack_Counter)++ ;            //Advance Counter to empty cell
                                            stack[*stack_Counter]=temp;            //Input into cell and advance
                                            (*stack_Counter)++;
                                            break;
                                         }
                                case '*':
                                case '/':
                                case '^':
                                case '#':{                                                       //equal precedence and left associative
                                            functionOutput[*function_Output_Counter]=stack[*stack_Counter];          //Pop '/' to output and advance output counter
                                            (*function_Output_Counter)++;
                                            stack[*stack_Counter]=temp;            //Push '/' onto stack and advance stack Counter
                                            (*stack_Counter)++;
                                            break;
                                         }
                                default : (*stack_Counter)++; stack[*stack_Counter]=temp; (*stack_Counter)++; break;
                                }

                            }
                            else{stack[*stack_Counter]=temp; (*stack_Counter)++; break;}
                            break;
                        }
            case '+':
            case '-': {
                        if(*stack_Counter>0)                                                  //Prevents stack underflow
                        {
                            (*stack_Counter)--;                                                 //Return counter to the last input
                            switch(*stack[*stack_Counter])
                            {
                                case '-':
                                case '+':
                                case '*':
                                case '/':
                                case '^':
                                        {
                                            functionOutput[*function_Output_Counter]=stack[*stack_Counter];          //Pop '*' to output and advance output counter
                                            (*function_Output_Counter)++;
                                            stack[*stack_Counter]=temp;                    //Push '/' onto stack and advance stack Counter
                                            (*stack_Counter)++;
                                            break;
                                        }
                                default : (*stack_Counter)++;stack[*stack_Counter]=temp; (*stack_Counter)++; break;
                            }

                        }
                        else
                        {
                            temp[0]=*inputLocal;
                            stack[*stack_Counter]=temp;

                            (*stack_Counter)++; break;
                        }
                        break;
                        }
            case '#':{
                         stack[*stack_Counter]=temp;                    //Push '#' onto stack and advance stack Counter
                        (*stack_Counter)++;
                        break;
                    }
            default : break;


            }


    inputLocal++;
    *inputString=inputLocal;                                                             // advances the input string pointer to the next character
}
