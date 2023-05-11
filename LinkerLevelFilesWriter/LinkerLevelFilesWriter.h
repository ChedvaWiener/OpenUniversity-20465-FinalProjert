/*
// Created by Dell on 27/02/2022.
*/

#ifndef FINALPROJECT_LINKERLEVELFILESWRITER_H
#define FINALPROJECT_LINKERLEVELFILESWRITER_H
#define WRITE_LINE_LENGTH 100
#define MAX_LENGTH_FOR_FILE_NAME 1024
#endif /* FINALPROJECT_LINKERLEVELFILESWRITER_H */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../DataStructs/List/List.h"
#include "../DataStructs/Dictionary/Dictionary.h"
#include "BuildMemoryViewFromAmFile.h"

/** Creates the ".obj", ".ent" and ".ext" output files. */
void createBinaryAndLinkerFiles(FILE* amFile, char* fileName);

void writeEntriesFile(List* entryList, FILE* entriesFile, int instructionCounter);

void writeExternFile(List* externList, FILE* externalsFile, int instructionCounter);

void writeObjFile(List* memViewList, FILE* objectFile, int instructionCounter);

void formatMachineCode(char* line, unsigned long binary);

void freeAllTheMemory(List* memViewList, List* entryList, List* externList, Dictionary *symbolTable);

void freeOperationNodesInList(List *opsList, Dictionary *symbolTable);
