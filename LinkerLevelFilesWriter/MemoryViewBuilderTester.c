/*
// Created by Dell on 08/03/2022.
*/

#include "MemoryViewBuilderTester.h"

int test1(FILE *amFile)
{
    Operation* operation;
    Node* current;
    List* memViewList = newList();
    List* entryList = newList();
    List* externList = newList();
    Dictionary* symbolTable = startDictionary(DICTIONARY_SIZE);
    buildMemoryView(memViewList, entryList, externList, symbolTable, amFile);
    current = memViewList->head;
    while (current != NULL)
    {
        operation = (Operation*)(current->data);
        printOperation(operation);
        current = current->next;
    }

    return 0;
}

void printOperation(Operation* operation)
{
    printf("\n" \
                  " isDecoded: %d\n"\
                  " isComment: %d\n"\
                  " isLabel:   %d\n"\
                  " blankline: %d\n"\
                  " label:     %s\n"\
                  " isDirective %d\n"\
                  " directiveDestiny: %d\n"\
                  " labelType:  %d\n"\
                  " lines:      %d\n"\
                  " instructionCounter: %d\n"\
                  " lineDecoded[0] %s\n"\
                  " lineDecoded[1] %s\n"\
                  " lineDecoded[2] %s\n"\
                  " lineDecoded[3] %s\n"\
                  " lineDecoded[4] %s\n"\
                  " lineDecoded[5] %s\n",
                  operation->isDecoded,
                  operation->isComment,
                  operation->isLabel,
                  operation->blankLine,
                  operation->label != NULL ? operation->label : "EMPTY",
                  operation->isDirective,
                  operation->directiveDestiny,
                  operation->isRelocatableDataLabel,
                  operation->lines,
                  operation->instructionCounterAddress - 100 + operation->lines,
                  longTo20DigitBinary(operation->lineDecoded[0]),
                  longTo20DigitBinary(operation->lineDecoded[1]),
                  longTo20DigitBinary(operation->lineDecoded[2]),
                  longTo20DigitBinary(operation->lineDecoded[3]),
                  longTo20DigitBinary(operation->lineDecoded[4]),
                  longTo20DigitBinary(operation->lineDecoded[5])
    );
    printf(" dataDecoded skipped\n"\
                  " sourceLineNum:  %d\n"\
                  " sourceLabel:    %s\n"\
                  " destinationLineNum: %d\n"\
                  " destinationLabel:   %s\n"\
                  " instructionCounterAddress: %d\n"\
                  " dataCounterAddress:        %d\n"\
                  " address:                   %d\n"\
                  " originCodeLineNum:         %d\n"\
                  "\n",

            /* operation->data decoded */
                  operation->SourceLineNum,
                  operation->sourceLabel != NULL ? operation->sourceLabel : "EMPTY",
                  operation->destinationLineNum,
                  operation->destinationLabel != NULL ? operation->destinationLabel : "EMPTY",
                  operation->instructionCounterAddress,
                  operation->dataCounterAddress,
                  operation->address,
                  operation->originCodeLineNum
            );
}

char* longTo20DigitBinary (unsigned long toConvert)
{
    char* result;
    unsigned long mask = 1;
    int i;
    result = (char*) malloc(40);
    for (i = 0; i < 20; i++)
    {
        if (toConvert & mask)
        {
            *(result + (39 - (2 * i))) = '1';
            *(result + (39 - (1 + (2 * i)))) = ' ';
        }
        else
        {
            *(result + (39 - (2 * i))) = '0';
            *(result + (39 - (1 + (2 * i)))) = ' ';
        }
        toConvert >>= 1;
    }
    *(result + i + i) = '\0';
    return result;
}