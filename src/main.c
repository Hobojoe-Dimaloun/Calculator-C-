/* **********************************
* Author:   Michael Jon O'Donnell
*           mo14776@my.bristol.ac.uk
*
*   Basic calculator
*   (+/-*^)
*************************************/

/* ******************************************
MIT License

Copyright (c) [2016] [Michael Jon O'Donnell]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "calculator.h"

int main()
{
    char input[100]={'\0'}, *inputString=NULL;

    while(input[0]!='E' && input[1]!='X' && input[2]!='I' && input[3]!='T')
    {
        printf("To exit type EXIT.\nTo perform arithmetic type ARITH\nTo go back type BACK\n");


        fgets(input, sizeof(input),stdin);
        inputString=input;

        if(input[0]=='A' && input[1]=='R' && input[2]=='I' && input[3]=='T' && input[4]=='H')
        {
            printf("Enter equation\n");
            while(input[0]!='B' && input[1]!='A' && input[2]!='C' && input[3]!='K')
            {
                fgets(input, sizeof(input),stdin);
                inputString=input;
                if(input[0]=='E' && input[1]=='X' && input[2]=='I' && input[3]=='T')
                {
                    break;
                }

                    arithmetic(inputString);

            }
        }

    }




    return(0);


}
