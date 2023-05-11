
/* Created by Dell on 20/01/2022. */

#ifndef FINALPROJECT_MACROLAYOUT_H
#define FINALPROJECT_MACROLAYOUT_H

#endif /* FINALPROJECT_MACROLAYOUT_H */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "../GlobalDefinitions.h"
#include "MacroList.h"
#define MAX_LINE_LENGTH 81


enum inputLineTypes {
    COMMENT, MACRO_DECLERATION, ASM_LINE, MACRO_END, MACRO_USE
};

/** Lays out the macros into output. Accepts input opened for reading, and output opened for Writing. */
FILE* layMacrosOut(FILE* input, FILE* output);

int checkLineType(char line[], MacroList* macroList);

void writeMacroIntoOutput(char *line, FILE *output, MacroList *macroList);

void writeLineIntoOutput(char* line, FILE* output);
/** returns true if first non white space char in line is ';' */
/*int isComment(char* line);*/
/** returns true if first word in line is "macro" */
int isMacroDecleration(char* line);
/** returns true if first word in line is "endm" */
int isMacroEnd(char* line);
/** returns true if a MacroNode in MacroList has the same name as the first word in line */
int isMacroInstance(char* line, MacroList* macroList);

void addMacro(FILE* input, char macroName[], MacroList* macroList, char* line);
/** Copies the first word in line into macroName. When isDecleration == true,
 *  the procedure will skip the word "macro" before finding the first word to
 *  copy (behavior is undefined if line does not contain "macro" as the first word). */
void setMacroName(char* line, char macroName[], int isDeclaration);
/** Builds the macro body string. Reads lines from input into macroBody until macro ending.
 *  The procedure manipulates a pointer to the macroBody string, and therefore needs a
 *  pointer to that pointer as an argument. */
void setMacroBody(FILE* input, char** macroBody);
