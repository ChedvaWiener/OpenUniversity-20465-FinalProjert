
/* Created by Yehuda Klein on 21/01/2022. */


#ifndef FINALPROJECT_ASSEMBLER_H
#define FINALPROJECT_ASSEMBLER_H

#endif /* FINALPROJECT_ASSEMBLER_H */


#include <stdio.h>
#include <string.h>
#include "MacroLayout/MacroLayout.h"
#include "LinkerLevelFilesWriter/LinkerLevelFilesWriter.h"

#define MAX_INPUT_FILE_NAME_LENGTH 1024
#define INPUT_FILE_NAME_TOO_LONG "ERROR: Name of input file too long. Must be less than 1000 characters."

int assemble(int argc, char *argv[]);
void assembleFile(char*);
/** Creates a 'path' for the .am file, with '"' at the beginning and end. */
char* createAmFileName(char* fileName, char amFileName[]);