/*
// Created by user on 28/02/2022.
*/

#include "error.h"

FILE *errorFile;

int currentFileContainsErrors = FALSE;

void printToErrorFile(char *line)
{
    fprintf(errorFile, "\n\n%s:\n", line);
}

void printError(int lineNum, char *errorMsg)
{
    fprintf(errorFile, "Error in line %d: %s.\n",lineNum, errorMsg);
    currentFileContainsErrors = TRUE;
}

void openErrorFileForWriting()
{
    if(!(errorFile=fopen("errorsFile.txt","w")))
    {
        printf("Cannot open error file. \n");
        exit(-1);
    }
}

void closeErrorFileForWriting()
{
    fclose(errorFile);
}

void openErrorFileForReading()
{
    if(!(errorFile=fopen("errorsFile.txt","r")))
    {
        printf("Cannot open error file. \n");
        exit(-1);
    }
}

#define ERROR_LINE_BUFFER_SIZE 1000
void printErrorFileContents()
{
    char line[ERROR_LINE_BUFFER_SIZE];
    {
        while(!feof(errorFile))
        {
            if(fgets(line, ERROR_LINE_BUFFER_SIZE, errorFile))
            {
                printf("%s", line);
            }
        }
    }
}

void closeErrorFileForReading()
{
    fclose(errorFile);
}

int doesCurrentFileContainErrors()
{
    return currentFileContainsErrors;
}

void setCurrentFileContainErrors(int booleanValue)
{
    currentFileContainsErrors = booleanValue;
}

