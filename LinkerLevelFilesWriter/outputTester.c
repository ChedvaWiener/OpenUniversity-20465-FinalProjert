/*
// Created by Dell on 10/03/2022.
*/

#include "outputTester.h"


void testObjFile()
{
    void quickCheck(FILE* objOutout);
    char expectedLine[100];
    char output[100];
    FILE* expectedObj,* objOutput;
    int lineNum = -1;
    expectedObj = fopen("expectedObj", "r");
    objOutput = fopen("test3.obj", "r");

    quickCheck(expectedObj);
    fseek(expectedObj, 0, SEEK_SET);
    while(!feof(expectedObj))
    {
        fgets(expectedLine, 100, expectedObj);
        fgets(output, 100, objOutput);

        if(strcmp(expectedLine, output))
        {
            printf("    problem: line %d\n    expected: %s    output:   %s\n\n", lineNum, expectedLine, output);
        }
        lineNum++;
    }
}

void quickCheck(FILE* expectedObj)
{
    char temp;
    int isOk = 1;
    while (!feof(expectedObj))
    {
        temp = getc(expectedObj);
        if ((temp > 127 || temp < 0) && temp != EOF)
        {
            isOk = 0;
        }
    }

    if(isOk)
    {
        printf("\n\n\n\n it's ok\n\n\n\n");
    }
    else
    {
        printf("\n\n\n\nthose motherfuckers.....\n\n\n\n");
    }
}