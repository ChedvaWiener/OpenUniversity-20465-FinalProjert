/*
// Created by Dell on 27/02/2022.
*/

#include "BuildMemoryViewFromAmFile.h"


static int lineNum;
static int instructionCounter;
static int dataCounter;

int buildMemoryView(List* memViewList, List* entryList, List* externList, Dictionary* symbolTable, FILE* amFile)
{
    List* instructionList;
    List* dataList;
    Operation* operation;
    char* line;
    lineNum = 0;
    instructionCounter = INSTRUCTION_COUNTER_START_VALUE;
    dataCounter = 0;
    instructionList = newList();
    dataList = newList();
    line = (char*)malloc(N);
    fseek(amFile, 0, SEEK_SET);
    while (!feof(amFile))
    {
        if(fgets(line, N, amFile))
        {
            lineNum++;
            operation = StartDecoding(line, lineNum);
            operation->originCodeLineNum = lineNum;
            addOperationToMemView(operation, symbolTable, instructionList, dataList, entryList, externList);
        }
        else if(*line != '\0')
        {
            lineNum++;
            printf("ERROR: In getting line %d from amFile in"\
                   "buildMemoryView(List* memViewList, Dictionary* symbolTable, FILE* amFile)\n", lineNum);
        }
    }

    concatDataAndInstructionsToCompleteMemView(instructionList, dataList, memViewList);
    free(instructionList);
    free(dataList);
    addSymbolsToMemViewObjectsAsNumberValues(memViewList, symbolTable, externList);
    updateEntryAddresses(entryList, symbolTable);
    fclose(amFile);

    return instructionCounter - INSTRUCTION_COUNTER_START_VALUE;
}

void addOperationToMemView(Operation* operation, Dictionary* symbolTable, List* instructionList, List* dataList, List* entryList, List* externList)
{
    if (operation->isComment)
    {
        freeUnusedOperations(operation);
        return; /* ignore comment */
    }
    if (operation->isDirective)
    {
        addDirectiveOperationToMemView(operation, symbolTable, instructionList, dataList, entryList, externList);
    }
    else
    {
        addInstructionOperationToMemView(operation, symbolTable, instructionList, dataList);
    }
}

void addDirectiveOperationToMemView(Operation* operation, Dictionary* symbolTable, List* instructionList, List* dataList, List* entryList, List* externList)
{
    switch (operation->directiveDestiny)
    {
        case STRING:
            addDataToDataList(operation, symbolTable, dataList);
            break;
        case DATA:
            addDataToDataList(operation, symbolTable, dataList);
            break;
        case ENTRY:
            addEntryToEntryList(operation, symbolTable, entryList);
            break;
        case EXTERN:
            declareLabelAsExtern(operation, symbolTable, externList);
            break;
    }
}

void addDataToDataList(Operation* operation, Dictionary* symbolTable, List* dataList)
{
    char* dataListId;
    Node* data;
    Operation* labelFromDict;
    if (operation->isLabel)
    {
        labelFromDict = (Operation*)getDataMemberById(operation->label, symbolTable);
        if(labelFromDict && labelFromDict->isDeclaredNonEntry)
        {
            handleMultipleLabelDeclarationError(operation, lineNum);
            return;
        }
        else if (labelFromDict)
        {
            labelFromDict->isDeclaredNonEntry = TRUE;
            labelFromDict->isRelocatableDataLabel = TRUE;
            labelFromDict->dataCounterAddress = dataCounter;
            labelFromDict->directiveDestiny = DATA;
        }
        else
        {
            operation->isDeclaredNonEntry = TRUE;
            operation->isRelocatableDataLabel = TRUE;
            addDataMember(operation->label, operation, symbolTable);
        }
    }
    dataListId = (char*)malloc(MAX_ID_BY_LINE_NUM_LENGTH);
    sprintf(dataListId, "%d", lineNum);
    data = newNode(dataListId, operation);
    addNode(dataList, data);
    free(dataListId);
    operation->dataCounterAddress = dataCounter;
    dataCounter += operation->lines;
}

void addEntryToEntryList(Operation* operation, Dictionary* symbolTable, List* entryList)
{
    char* entryListId;
    Node* entry;
    Operation* labelFromDict;
    labelFromDict = (Operation*)getDataMemberById(operation->label, symbolTable);
    if (labelFromDict && labelFromDict->directiveDestiny == ENTRY)
    {
        /* label with same name was already declared as entry */
        freeUnusedOperations(operation);
        return;
    }
    else if (labelFromDict && labelFromDict->directiveDestiny == EXTERN)
    {
        handleRedeclarationOfExternalAsEntry(operation);
        return;
    }
    if(labelFromDict == NULL)
    {
        addDataMember(operation->label, operation, symbolTable);
    }
    entryListId = (char*)malloc(MAX_ID_BY_LINE_NUM_LENGTH);
    sprintf(entryListId, "%d", lineNum);
    entry = newNode(entryListId, operation);
    addNode(entryList, entry);
    free(entryListId);
}

void handleRedeclarationOfExternalAsEntry(Operation* operation)
{
    char* errorMsg = (char*)malloc(WRONG_REDECLARATION_ERROM_MSG_LENGTH);
    sprintf(errorMsg, "On line %d, redeclaration of extern label %s as entry.\n", operation->originCodeLineNum, operation->label);
    printError(operation->originCodeLineNum, errorMsg);
    free(errorMsg);
}

void declareLabelAsExtern(Operation* operation, Dictionary* symbolTable, List* externList)
{
    Operation* labelFromDict;
    labelFromDict = (Operation*)getDataMemberById(operation->label, symbolTable);
    if (labelFromDict && labelFromDict->directiveDestiny == EXTERN)
    {
        /* label with same name was already declared as entry */
        free(operation);
        return;
    }
    else if (labelFromDict && labelFromDict->directiveDestiny == ENTRY)
    {
        handleRedeclarationOfEntryAsExternal(operation);
        return;
    }
    addDataMember(operation->label, operation, symbolTable);
}

void handleRedeclarationOfEntryAsExternal(Operation* operation)
{
    char* errorMsg = (char*)malloc(WRONG_REDECLARATION_ERROM_MSG_LENGTH);
    sprintf(errorMsg, "On line %d, redeclaration of entry label %s as extern.\n", operation->originCodeLineNum, operation->label);
    printError(operation->originCodeLineNum, errorMsg);
    free(errorMsg);
}

void addInstructionOperationToMemView(Operation* operation, Dictionary* symbolTable, List* instructionList, List* dataList)
{
    char* instructionListId;
    Node* instruction;
    Operation* labelFromDict;
    if (operation->isLabel)
    {
        labelFromDict = getDataMemberById(operation->label, symbolTable);
        if(labelFromDict && labelFromDict->isDeclaredNonEntry)
        {
            handleMultipleLabelDeclarationError(operation, lineNum);
            return;
        }
        else if (labelFromDict)
        {
            labelFromDict->isDeclaredNonEntry = TRUE;
            labelFromDict->isRelocatableDataLabel = FALSE;
            labelFromDict->instructionCounterAddress = instructionCounter;
        }
        else
        {
            operation->isDeclaredNonEntry = TRUE;
            operation->isRelocatableDataLabel = FALSE;
            addDataMember(operation->label, operation, symbolTable);
        }
    }
    instructionListId = (char*)malloc(MAX_ID_BY_LINE_NUM_LENGTH);
    sprintf(instructionListId, "%d", lineNum);
    instruction = newNode(instructionListId, operation);
    addNode(instructionList, instruction);
    free(instructionListId);
    operation->instructionCounterAddress = instructionCounter;
    instructionCounter += operation->lines;
}

Operation* containsMultipleLabelDeclaration(Operation* operation, Dictionary* symbolTable)
{
    Operation* labelFromDict;
    labelFromDict = (Operation*)getDataMemberById(operation->label, symbolTable);
    if(labelFromDict && labelFromDict->isDeclaredNonEntry)
    {
        return labelFromDict;
    }
    else
    {
        return NULL;
    }
}

void handleMultipleLabelDeclarationError(Operation* operation, int lineNum)
{
    char* errorMsg = (char*)malloc(MULTIPLE_DECLARATION_ERROR_MSG_LENGTH);
    sprintf(errorMsg, "On line %d, multiple declaration of label %s \n", lineNum, operation->label);
    printError(lineNum, errorMsg);
    free(errorMsg);
}

void concatDataAndInstructionsToCompleteMemView(List* instructionList, List* dataList, List* memViewList)
{
    if(isEmpty(instructionList) && isEmpty(dataList))
    {
        return;
    }
    else if (isEmpty(instructionList))
    {
        memViewList = dataList;
    }
    else if (isEmpty(dataList))
    {
        memViewList = instructionList;
    }
    else
    {
        memViewList->head = instructionList->head;
        memViewList->tail = dataList->tail;
        instructionList->tail->next = dataList->head;
        dataList->head->prev = instructionList->tail;
    }
}

void addSymbolsToMemViewObjectsAsNumberValues(List *memViewList, Dictionary *symbolTable, List *externList)
{

    Node* current;
    Operation* label;
    Operation* currentOperation;
    current = memViewList->head;
    while (current != NULL)
    {
        currentOperation = (Operation*)current->data;
        if (currentOperation->sourceLabel != NULL)
        {
            label = getDataMemberById(currentOperation->sourceLabel, symbolTable);
            if (label == NULL)
            {
                handleUseOfNonExistentLabelInSourceOperand(currentOperation);
            }
            else if (label->isDirective && label->directiveDestiny == EXTERN)
            {
                insertExternalAddressPlaceHoldersToSource(currentOperation, label, externList);
            }
            else
            {
                insertRelocatableAddressesToSource(currentOperation, label);
            }
        }
        if (currentOperation->destinationLabel != NULL)
        {
            label = getDataMemberById(currentOperation->destinationLabel, symbolTable);
            if (label == NULL)
            {
                handleUseOfNonExistentLabelInDestinationOperand(currentOperation);
            }
            else if (label->isDirective && label->directiveDestiny == EXTERN)
            {
                insertExternalAddressPlaceHoldersToDestination(currentOperation, label, externList);
            }
            else
            {
                insertRelocatableAddressesToDestination(currentOperation, label);
            }
        }

        current = current->next;
    }
}

void handleUseOfNonExistentLabelInSourceOperand(Operation* currentOperation)
{
    char* errorMsg = (char*)malloc(NON_EXISTANT_LABEL_ERROR_MSG_LENGTH);
    sprintf(errorMsg, "On line %d, use of non existant label %s in source operand address.\n", currentOperation->originCodeLineNum, currentOperation->sourceLabel);
    printError(currentOperation->originCodeLineNum, errorMsg);
    free(errorMsg);
}

void handleUseOfNonExistentLabelInDestinationOperand(Operation* currentOperation)
{
    char* errorMsg = (char*)malloc(NON_EXISTANT_LABEL_ERROR_MSG_LENGTH);
    sprintf(errorMsg, "On line %d, use of non existent label %s in destination operand address.\n", currentOperation->originCodeLineNum, currentOperation->sourceLabel);
    printError(currentOperation->originCodeLineNum, errorMsg);
    free(errorMsg);
}

void insertRelocatableAddressesToSource(Operation* currentOperation, Operation* label)
{
    int address;
    if (label->isRelocatableDataLabel)
    {
        address = label->dataCounterAddress + instructionCounter;
    }
    else
    {
        address = label->instructionCounterAddress;
    }
    currentOperation->lineDecoded[currentOperation->SourceLineNum] = address - (address % 16) + IS_RELOCATABLE_ADDRESS;
    currentOperation->lineDecoded[currentOperation->SourceLineNum + 1] = (address % 16) + IS_RELOCATABLE_ADDRESS;
}

void insertExternalAddressPlaceHoldersToSource(Operation* currentOperation, Operation* label, List* externList)
{
    char* externListId;
    Node* external;
    int insertAddress;
    currentOperation->lineDecoded[currentOperation->SourceLineNum] = IS_EXTERNAL_ADDRESS;
    currentOperation->lineDecoded[currentOperation->SourceLineNum + 1] = IS_EXTERNAL_ADDRESS;

    insertAddress = currentOperation->instructionCounterAddress + currentOperation->SourceLineNum;
    currentOperation->sourceBaseAddressToInsertForExtern = insertAddress;
    currentOperation->sourceAddressOffsetToInsertForExtern = insertAddress + 1;
    externListId = (char*)malloc(MAX_ID_BY_LINE_NUM_LENGTH);
    sprintf(externListId, "%d", currentOperation->originCodeLineNum);
    external = newNode(externListId, currentOperation);
    addNode(externList, external);
    free(externListId);
}

void insertRelocatableAddressesToDestination(Operation* currentOperation, Operation* label)
{
    int address;
    if (label->isRelocatableDataLabel)
    {
        address = label->dataCounterAddress + instructionCounter;
    }
    else
    {
        address = label->instructionCounterAddress;
    }
    currentOperation->lineDecoded[currentOperation->destinationLineNum] = address - (address % 16) + IS_RELOCATABLE_ADDRESS;
    currentOperation->lineDecoded[currentOperation->destinationLineNum + 1] = (address % 16) + IS_RELOCATABLE_ADDRESS;
}

void insertExternalAddressPlaceHoldersToDestination(Operation* currentOperation, Operation* label, List* externList)
{
    char* externListId;
    Node* external;
    int insertAddress;
    currentOperation->lineDecoded[currentOperation->destinationLineNum] = IS_EXTERNAL_ADDRESS;
    currentOperation->lineDecoded[currentOperation->destinationLineNum + 1] = IS_EXTERNAL_ADDRESS;

    insertAddress = currentOperation->instructionCounterAddress + currentOperation->destinationLineNum;
    currentOperation->destinationBaseAddressToInsertForExtern = insertAddress;
    currentOperation->destinationAddressOffsetToInsertForExtern = insertAddress + 1;
    externListId = (char*)malloc(MAX_ID_BY_LINE_NUM_LENGTH);
    sprintf(externListId, "%d", currentOperation->originCodeLineNum);
    external = newNode(externListId, currentOperation);
    addNode(externList, external);
    free(externListId);
}

void updateEntryAddresses(List* entryList, Dictionary* symbolTable)
{
    char warning[NO_CORRESPONDING_ENRTY_WARNING_LENGTH];
    Node* current;
    Operation* entry, * labelFromDict;

    current = entryList->head;
    while(current != NULL)
    {
        entry = (Operation*)(current->data);
        labelFromDict = (Operation*) getDataMemberById(entry->label, symbolTable);

        if(!labelFromDict)
        {
            sprintf(warning, "Warning: Label %s was declared as entry, but is not declared in file.\n", entry->label);
            printToErrorFile(warning);
        }
        else
        {
            entry->directiveDestiny = labelFromDict->directiveDestiny;
            entry->instructionCounterAddress = labelFromDict->instructionCounterAddress;
            entry->dataCounterAddress = labelFromDict->dataCounterAddress;
        }

        current = current->next;
    }
}

void freeUnusedOperations(Operation* operation)
{
    free(operation->label);
    free(operation->dataDecoded);
    free(operation->sourceLabel);
    free(operation->destinationLabel);
    free(operation);
}