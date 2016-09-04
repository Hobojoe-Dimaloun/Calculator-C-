/* **********************************
* Author:   Michael Jon O'Donnell
*           mo14776@my.bristol.ac.uk
*
*   Basic calculator
*   (+/-*^)
*************************************/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "calculator.h"


// Defines operation positions in the modes array.
// Used for ease of reading
enum modes
{
    EXIT, Exit, BACK,
    back, ARITH, arith
};

int main()
{
    // Defines operation modes
    static const char *Modes[]=
    {
        "EXIT","exit","BACK",
        "back","ARITH","arith"
    };


    char input[100]={'\0'}, *inputString=NULL;
    int i;

    do
    {
        printf("To exit type EXIT.\nTo perform arithmetic type ARITH\nTo go back type BACK\n");

        // Get user input then remove the newline symbol
        fgets(input, sizeof(input),stdin);
        for(i=0;i<100;i++)
        {
            if(input[i]=='\n'){input[i]='\0';   break;}
        }

        inputString=input;

        // Compare the Input to modes. If true performs arithmetic.
        if(strncmp(Modes[ARITH],inputString,5)==0 || strncmp(Modes[arith],inputString,5)==0)
        {
            printf("Enter equation\n");
            do
            {
                fgets(input, sizeof(input),stdin);
                for(i=0;i<100;i++)
                {
                    if(input[i]=='\n'){input[i]='\0';   break;}
                }
                inputString=input;

                // Allows for exit of program without returning to the first menu
                if(strncmp(Modes[EXIT],inputString,4)==0 || strncmp(Modes[Exit],inputString,4)==0)
                {
                    break;
                }
                      arithmetic(inputString);


            // Returns to previous menu if back is entered
            }while(strncmp(Modes[BACK],inputString,4)!=0 &&  strncmp(Modes[back],inputString,4)!=0);
        }

    }while(strncmp(Modes[EXIT],inputString,4)!=0 && strncmp(Modes[Exit],inputString,4)!=0);


    return(0);


}
