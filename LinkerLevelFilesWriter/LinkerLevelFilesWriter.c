/*
// Created by Dell on 27/02/2022.
*/

#include "LinkerLevelFilesWriter.h"


void createBinaryAndLinkerFiles(FILE* amFile, char* fileName)
{
    char entFileName[MAX_LENGTH_FOR_FILE_NAME];
    char extFileName[MAX_LENGTH_FOR_FILE_NAME];
    char objFileName[MAX_LENGTH_FOR_FILE_NAME];
    int instructionCounter;
    FILE *entriesFile, *externalsFile, *objectFile;
    List* memViewList = newList();
    List* entryList = newList();
    List* externList = newList();
    Dictionary* symbolTable = startDictionary(DICTIONARY_SIZE);
    instructionCounter = buildMemoryView(memViewList, entryList, externList, symbolTable, amFile);
    fclose(amFile);

    if (!doesCurrentFileContainErrors())
    {
        strcpy(entFileName, fileName);
        strcat(entFileName, ".ent");
        {
            entriesFile = fopen(entFileName, "w");
            writeEntriesFile(entryList, entriesFile, instructionCounter);
            fclose(entriesFile);
        }
        strcpy(extFileName, fileName);
        strcat(extFileName, ".ext");
        if(!isEmpty(externList))
        {
            externalsFile = fopen(extFileName, "w");
            writeExternFile(externList, externalsFile, instructionCounter);
            fclose(externalsFile);
        }
        strcpy(objFileName, fileName);
        strcat(objFileName, ".obj");
        if(!isEmpty(entryList))
            objectFile = fopen(objFileName, "w");
        writeObjFile(memViewList, objectFile, instructionCounter);
        fclose(objectFile);
    }

    freeAllTheMemory(memViewList, entryList, externList, symbolTable);
}

void writeEntriesFile(List* entryList, FILE* entriesFile, int instructionCounter)
{
    Node* current;
    Operation* entry;
    int address;
    current = entryList->head;
    while (current != NULL)
    {
        entry = (Operation*)(current->data);
        if (entry->directiveDestiny == DATA || entry->directiveDestiny == STRING)
        {
            address = instructionCounter + 100 + entry->dataCounterAddress;
        }
        else
        {
            address = entry->instructionCounterAddress;
        }
        fprintf(entriesFile, "%s,%d,%d\n", entry->label, address - (address % 16), (address % 16));

        current = current->next;
    }
}

void writeExternFile(List* externList, FILE* externalsFile, int instructionCounter)
{
    Node* current;
    Operation* external;
    current = externList->head;
    while (current != NULL)
    {
        external = (Operation*)(current->data);
        if(external->sourceBaseAddressToInsertForExtern > -1)
        {
            fprintf(externalsFile, "%s BASE %d\n"\
                                              "%s OFFSET %d\n\n",
                                              external->sourceLabel,
                                              external->sourceBaseAddressToInsertForExtern,
                                              external->sourceLabel,
                                              external->sourceAddressOffsetToInsertForExtern);
        }
        if(external->destinationBaseAddressToInsertForExtern > -1)
        {
            fprintf(externalsFile, "%s BASE %d\n"\
                                              "%s OFFSET %d\n\n",
                                              external->destinationLabel,
                                              external->destinationBaseAddressToInsertForExtern,
                                              external->destinationLabel,
                                              external->destinationAddressOffsetToInsertForExtern);
        }

        current = current->next;
    }
}

void writeObjFile(List* memViewList, FILE* objectFile, int instructionCounter)
{
    char line[WRITE_LINE_LENGTH];
    int i, instructionImageSize, dataImageSize;
    Operation* operation;
    Node* current;

    instructionImageSize = instructionCounter;
    operation = (Operation*)(memViewList->tail->data);
    dataImageSize = operation->dataCounterAddress + 1;
    fprintf(objectFile, "%d %d\n", instructionImageSize, dataImageSize);

    current = memViewList->head;
    while (current != NULL)
    {
        operation = (Operation*)(current->data);
        if(operation->isDirective && (operation->directiveDestiny == DATA || operation->directiveDestiny == STRING))
        {
            for(i = 0; i < operation->lines; i++)
            {
                formatMachineCode(line, *(operation->dataDecoded + i));
                fprintf(objectFile, "%0*d %s\n", 4, instructionCounter + 100 + operation->dataCounterAddress + i, line);
            }
        }
        else
        {
            for(i = 0; i < operation->lines; i++)
            {
                formatMachineCode(line, (operation->lineDecoded)[i]);
                fprintf(objectFile, "%0*d %s\n", 4, operation->instructionCounterAddress + i, line);
            }
        }

        current = current->next;
    }
}

#define MASK1 15
#define MASK2 (15 << 4)
#define MASK3 (15 << 8)
#define MASK4 (15 << 12)
#define MASK5 (15 << 16)
void formatMachineCode(char* line, unsigned long binary)
{
    sprintf(line, "A%x-B%x-C%x-D%x-E%x",
                (unsigned int)((binary & MASK5) >> 16),
                (unsigned int)((binary & MASK4) >> 12),
                (unsigned int)((binary & MASK3) >> 8),
                (unsigned int)((binary & MASK2) >> 4),
                (unsigned int)((binary & MASK1))
            );
}

void freeAllTheMemory(List* memViewList, List* entryList, List* externList, Dictionary *symbolTable)
{
    freeOperationNodesInList(memViewList, symbolTable);
    deleteList(memViewList);
    freeOperationNodesInList(entryList, symbolTable);
    deleteList(entryList);
    freeOperationNodesInList(externList, symbolTable);
    deleteList(externList);
    deleteDictionary(symbolTable);
}

void freeOperationNodesInList(List *opsList, Dictionary *symbolTable)
{
    Operation* operation;
    Node* current = opsList->head;
    while (current != NULL)
    {
        operation = (Operation*)(current->data);
        if(operation->label != NULL)
            free(operation->label);
        if(operation->dataDecoded != NULL)
            free(operation->dataDecoded);
        if(operation->sourceLabel != NULL)
            free(operation->sourceLabel);
        if(operation->destinationLabel != NULL)
            free(operation->destinationLabel);
        free(operation);
        current->data = NULL;

        current = current->next;
    }
}