
/* Created by Dell on 21/01/2022. */

#include "Assembler.h"
#include "Error/error.h"
#include "LinkerLevelFilesWriter/MemoryViewBuilderTester.h"
#include "LinkerLevelFilesWriter/outputTester.h"

int assemble(int argc, char *argv[])
{
    int inputNum = 1;

    openErrorFileForWriting();

    if(argc <= 1)
    {
        printf("ERROR: No arguments found\n");
        return -1;
    }

    while (inputNum < argc)
    {
        assembleFile(argv[inputNum]);
        inputNum++;
    }

    closeErrorFileForWriting();

    openErrorFileForReading();
    printErrorFileContents();
    closeErrorFileForReading();

    return 0;
}

void assembleFile (char* fileName)
{
    FILE *file, *amFile;
    char amFileName[MAX_INPUT_FILE_NAME_LENGTH];
    char errorFileBuffer[MAX_INPUT_FILE_NAME_LENGTH + 20];

    setCurrentFileContainErrors(FALSE);
    sprintf(errorFileBuffer, "Assembling file %s.\n", fileName);
    printToErrorFile(errorFileBuffer);

    if (strlen(fileName) > MAX_INPUT_FILE_NAME_LENGTH)
    {
        printf("%s %s", INPUT_FILE_NAME_TOO_LONG,fileName);
        return;
    }
    if (!(file = fopen(fileName, "r")))
    {
        printf("ERROR: Cannot find file name %s", fileName);
        return;
    }

    if (!(amFile = fopen(createAmFileName(fileName, amFileName), "w")))
    {
        printf("ERROR: Cannot create file %s.am", fileName);
        return;
    }
    /* amFile is generated from  the original input file during macro layout process */
    layMacrosOut(file, amFile);

    /* moving to work with amFile in read mode */
    fclose(amFile);
    fclose(file);
    if (!(amFile = fopen(amFileName, "r")))
    {
        printf("ERROR: Could not reopen %s for reading.", fileName);
        return;
    }

    createBinaryAndLinkerFiles(amFile, fileName);
    /*if (!(amFile = fopen(amFileName, "r")))
      {
          printf("ERROR: Could not reopen %s for reading.", fileName);
          return;
      }
      test1(amFile); */
    fclose(amFile);

    /* testObjFile(); */
}

char* createAmFileName(char* fileName, char amFileName[])
{
    strcpy(amFileName, fileName);
    strcat(amFileName, ".am");
    return amFileName;
}