/*
// Created by Dell on 27/02/2022.
*/

#include "../DataStructs/List/List.h"
#include "../DataStructs/Dictionary/Dictionary.h"
#include "../DataStructs/operation/operation.h"
#include "../DecodingOperations/decodingOperations.h"
#include "../Error/error.h"

#define MULTIPLE_DECLARATION_ERROR_MSG_LENGTH 150
#define NON_EXISTANT_LABEL_ERROR_MSG_LENGTH 150
#define WRONG_REDECLARATION_ERROM_MSG_LENGTH 150
#define NO_CORRESPONDING_ENRTY_WARNING_LENGTH 150
#define MAX_ID_BY_LINE_NUM_LENGTH 6
#define INSTRUCTION_COUNTER_START_VALUE 100
#define IS_RELOCATABLE_ADDRESS (1<<17)
#define IS_EXTERNAL_ADDRESS (1<<16)

#ifndef FINALPROJECT_BUILDMEMORYVIEWFROMAMFILE_H
#define FINALPROJECT_BUILDMEMORYVIEWFROMAMFILE_H

#endif /* FINALPROJECT_BUILDMEMORYVIEWFROMAMFILE_H */

int buildMemoryView(List* memViewList, List* entryList, List* externList, Dictionary* symbolTable, FILE* amFile);

void addOperationToMemView(Operation* operation, Dictionary* symbolTable, List* instructionList, List* dataList, List* entryList, List* externList);

void addDirectiveOperationToMemView(Operation* operation, Dictionary* symbolTable, List* instructionList, List* dataList, List* entryList, List* externList);

void addDataToDataList(Operation* operation, Dictionary* symbolTable, List* dataList);

void addEntryToEntryList(Operation* operation, Dictionary* symbolTable, List* entryList);

void declareLabelAsExtern(Operation* operation, Dictionary* symbolTable, List* externList);

void addInstructionOperationToMemView(Operation* operation, Dictionary* symbolTable, List* instructionList, List* dataList);

void concatDataAndInstructionsToCompleteMemView(List* instructionList, List* dataList, List* memViewList);

void addSymbolsToMemViewObjectsAsNumberValues(List *memViewList, Dictionary *symbolTable, List *externList);

Operation* containsMultipleLabelDeclaration(Operation* operation, Dictionary* symbolTable);

void handleMultipleLabelDeclarationError(Operation* operation, int lineNum);

void handleUseOfNonExistentLabelInSourceOperand(Operation* currentOperation);

void handleUseOfNonExistentLabelInDestinationOperand(Operation* currentOperation);

void handleRedeclarationOfExternalAsEntry(Operation* operation);

void handleRedeclarationOfEntryAsExternal(Operation* operation);

void insertExternalAddressPlaceHoldersToSource(Operation* currentOperation, Operation* label, List* externList);

void insertRelocatableAddressesToSource(Operation* currentOperation, Operation* label);

void insertExternalAddressPlaceHoldersToDestination(Operation* currentOperation, Operation* label, List* externList);

void insertRelocatableAddressesToDestination(Operation* currentOperation, Operation* label);

void updateEntryAddresses(List* entryList, Dictionary* symbolTable);

void freeUnusedOperations(Operation* operation);