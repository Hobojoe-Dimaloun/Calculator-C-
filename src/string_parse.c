#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "string_parse.h"

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

int constantsLetterRead(char **inputString)
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


int bannedLetterRead()
{
    // If this is used then the letter is 'banned' and -1 returned

    return(-1);
}
