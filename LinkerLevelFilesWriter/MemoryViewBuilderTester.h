/*
// Created by Dell on 08/03/2022.
*/

#ifndef FINALPROJECT_MEMORYVIEWBUILDERTESTER_H
#define FINALPROJECT_MEMORYVIEWBUILDERTESTER_H

#endif /* FINALPROJECT_MEMORYVIEWBUILDERTESTER_H */

#include "BuildMemoryViewFromAmFile.h"
#include "LinkerLevelFilesWriter.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int test1(FILE* amFile);

void printOperation(Operation* operation);

char* longTo20DigitBinary (unsigned long toConvert);