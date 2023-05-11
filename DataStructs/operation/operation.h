/*
// Created by user on 19/02/2022.
*/
#include  "../../GlobalDefinitions.h"

#ifndef FINALPROJECT_OPERATION_H
#define FINALPROJECT_OPERATION_H

typedef struct Operation
{
    int isDecoded; /* In case of Error */
    int isComment;
    int isLabel;
    int blankLine;
    char* label; /* if the operation is entry so the label is the entry label, and not the label line */
    int isDirective;
    int directiveDestiny; /* String, data , external or internal*/
    int isRelocatableDataLabel;
    int lines; /* How many lines are needed to decoding */
    long lineDecoded[6]; /* The operation after encoding */
    long *dataDecoded;/*For data or string*/
    int SourceLineNum;
    char* sourceLabel; /*The source label */
    int destinationLineNum;
    char* destinationLabel; /*The destination label */
    int isDeclaredNonEntry;
    int instructionCounterAddress;
    int dataCounterAddress;
    int address;
    int originCodeLineNum;
    int baseAddress;
    int addressOffset;
    int sourceBaseAddressToInsertForExtern;
    int sourceAddressOffsetToInsertForExtern;
    int destinationBaseAddressToInsertForExtern;
    int destinationAddressOffsetToInsertForExtern;
} Operation;

#endif /* FINALPROJECT_BUILDMEMORYVIEWFROMAMFILE_H */
