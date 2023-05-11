
/* Created by Dell on 20/01/2022. */

#include "MacroLayout.h"

FILE* layMacrosOut(FILE* input, FILE* output)
{
    int lineType;
    char line[MAX_LINE_LENGTH];
    char macroName[MAX_LINE_LENGTH];
    MacroList *macroList;
    macroList = newMacroList();
    while (!feof(input))
    {
        if(fgets(line, MAX_LINE_LENGTH, input))
        {
            lineType = checkLineType(line, macroList);
            switch (lineType)
            {
                /*case COMMENT:*/
                    /*writeLineIntoOutput(line, output);*/
                case MACRO_DECLERATION:
                    addMacro(input, macroName, macroList, line);
                    break;
                case MACRO_USE:
                    writeMacroIntoOutput(line, output, macroList);
                    break;
                case ASM_LINE:
                        writeLineIntoOutput(line, output);
                    break;

                default: break;
            }
        }
    }
    return  output;
}
int checkLineType(char line[], MacroList* macroList)
{
    /*if (isComment(line))
        //return COMMENT;*/
    if (isMacroDecleration(line))
        return MACRO_DECLERATION;
    if (isMacroEnd(line))
        return MACRO_END;
    if (isMacroInstance(line, macroList))
        return MACRO_USE;
    else
        return ASM_LINE;
}
/*
int isComment(char* line)
{
    while(isspace(*line))
        line++;
    return (*line == ';');
}*/

int isMacroDecleration(char* line)
{

    while(isspace(*line))
        line++;
    if(!strncmp(line, "macro", 5) && isspace(*(line + 5)))
        return TRUE;
    else
        return FALSE;
}

int isMacroInstance(char* line, MacroList* macroList)
{
    char* firstWord = NULL;
    int length = 0, isMacroInstance;
    while (isspace(*line))
        line++;
    while (!isspace(*(line + length)))
        length++;
    firstWord = (char*)malloc(length + 1);
    if (!firstWord)
    {
        printf("ERROR: On allocating memory for firstWord in isMacroInstance().\n");
        exit(-1);
    }
    strncpy(firstWord, line, length);
    *(firstWord + length) = '\0';

    isMacroInstance = doesMacroExistInList(firstWord, macroList);

    free(firstWord);
    return isMacroInstance;
}

/************* start macro adding functionality ****************************/

void addMacro(FILE* input, char macroName[], MacroList* macroList, char* line)
{
    char* macroBody;
    macroBody = (char*)malloc(0);

    setMacroName(line, macroName, TRUE);

    setMacroBody(input, &macroBody);

    addMacroNode(macroName, macroBody, macroList);

    free(macroBody);
}

void setMacroName(char* line, char macroName[], int isDeclaration)
{
    int i = 0;
    while (isspace(*line))
        line++;
    if(isDeclaration)
        line += 5; /* skip over "macro" part of declaration */
    while (isspace(*line))
        line++;

    while (!isspace(*line)) /* copy name in line into macroName */
    {
        macroName[i] = *line;
        line++, i++;
    }
    macroName[i] = '\0';
}

void setMacroBody(FILE* input, char** macroBody)
{
    char line[MAX_LINE_LENGTH];

    fgets(line, MAX_LINE_LENGTH, input);
    if(isMacroEnd(line))
        return;

    *macroBody = (char*) realloc(*macroBody, strlen(line) + 1);
    strcpy(*macroBody, line);   /* initialize macroBody str so loop can concat */
    fgets(line, MAX_LINE_LENGTH, input);
    while (!isMacroEnd(line))
    {
        *macroBody = (char*) realloc(*macroBody, strlen(*macroBody) + strlen(line) + 1);
        strcat(*macroBody, line);
        fgets(line, MAX_LINE_LENGTH, input);
    }
}

int isMacroEnd(char* line)
{
    while(isspace(*line))
        line++;
    if(!strncmp(line, "endm", 4) && isspace(*(line + 4)))
        return TRUE;
    else
        return FALSE;
}


/*************** macro adding functionality end ****************************/

void writeMacroIntoOutput(char *line, FILE *output, MacroList *macroList)
{
    char macroName[MAX_LINE_LENGTH];
    char* macroBody;

    setMacroName(line, macroName, FALSE);

    macroBody = findMacro(macroName, macroList);

    fprintf(output, "%s", macroBody);
}

void writeLineIntoOutput(char *line, FILE* output)
{
    fprintf(output, "%s", line);
}

