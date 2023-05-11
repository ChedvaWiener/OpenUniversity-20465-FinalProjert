/*
 Created by user on 28/02/2022.
*/

#include <stdio.h>
#include <process.h>
#include "../GlobalDefinitions.h"

#ifndef FINALPROJECT_ERROR_H
#define FINALPROJECT_ERROR_H


#endif /*FINALPROJECT_ERROR_H*/

/** Closing the error file */
void closeErrorFileForWriting();

/** Opening the error file */
void openErrorFileForWriting();

/** Printing to error file - not an error message  */
void printToErrorFile(char *line);

/** Printing to error file - an error message   */
void printError(int lineNum, char *errorMsg);

void openErrorFileForReading();

void printErrorFileContents();

void closeErrorFileForReading();

int doesCurrentFileContainErrors();

void setCurrentFileContainErrors(int booleanValue);
