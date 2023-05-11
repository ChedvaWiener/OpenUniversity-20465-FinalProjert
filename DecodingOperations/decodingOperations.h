/*
// Created by user on 19/02/2022.
*/
#include "../DataStructs/operation/operation.h"
#include "../DataStructs/command/commands.h"
#include "../Error/error.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define COMMANDS_TABLE_LENGTH 16
#define GROUP_1_END_INDEX 4
#define GROUP_2_END_INDEX 13
#define MAX_NUM 32767
#define MIN_NUM -32767
#define MASK 0xFFFF
#define NOT_A_VALUE -1

#ifndef FINALPROJECT_DECODINGOPERATIONS_H
#define FINALPROJECT_DECODINGOPERATIONS_H

enum directives {DATA, STRING, REGULAR_LABEL, ENTRY, EXTERN};

enum aer {INDEX,DIRECT,IMMEDIATE,REGISTER};

#endif /* FINALPROJECT_DECODINGOPERATIONS_H */



/** Gets a line from the am file, line number  and returns appropriate operation struct */
Operation* StartDecoding(char* line, int lineNum);

/** Decodes the line into operation struct:
 1. Checks if the line is blank line.
 2. Checks if the line is  comment
 3. Checks if there is a label and if so extract the label.
 4. checks if the line is a directive line and if so sends to function according to the directive type.
 5. Otherwise sends to function that extracts command */
void decoding (Operation *poperation,char* line, int lineNum);

/** Sets all operation values to false or null */
void initializeOperationValues(Operation* operation);

/** Checks if there are only white space in the line */
int isBlankLine(char* line);

/** Deletes white space from the beginning of the line */
void deleteWhiteSpace(char* line);

/** Checks if the line is comment */
int isComment (char* line);

/** Checks if there  is a directive in the line */
int isDirectives (char* line);

/** Returns the directive type */
int directiveType (char* line);

/** Decodes data directives into the operation struct*/
void dataDecoding(Operation *poperation,char *line, int lineNum);

/** Decodes string directives into the operation struct*/
void srtringDecoding(Operation *poperation,char *line, int lineNum);

/** Decodes entry  directives into the operation struct*/
void entryDecoding(Operation *poperation,char *line, int lineNum);

/** Decodes extern  directives into the operation struct*/
void externDecoding(Operation *poperation,char *line, int lineNum);

/** Extracts the first number int lineTmp */
int extractNum(char *lineTmp, int lineNum);

/** Checks if there is another number in line */
int isAnotherNum(char *lineTmp);

/** Checks if there ia a comma anf if there is only one comma */
int isLegalComma(char *lineTmp);

/** Checks if the line is empty, means there are no unnecessary characters */
int lineIsEmpty(char *lineTmp);

/** Extracts the label of entry and extern */
int isLegalLabelEntry(char *lineTmp, int lineNum,Operation *poperation);

/** Extracts regular label */
int isLegalLabel(char *lineTmp, int lineNum,Operation *poperation);

/** Checks is the first word in the line is label */
int isLabel (char* line);

/** Extracts the label */
void extractLabel(Operation *poperation,char *line, int lineNum);

/** Extracts the command */
void extractCommand(Operation *poperation,char *line, int lineNum);

/** Extracts the first word in the line */
void extractWord(char *line,char *word);

/** Extracts the operand */
int extractOperand(char *line,char *oprand,int lineNum);

/** Add the operands to the operation struct
 1. Checks the  source operand addressing method
 2. According to the addressing method adds lines to the struct
 3. Does the same to the destination operand*/
int addOperands(Operation *poperation,Command command,char *sourceOperand, char *destinationOperand, int lineNum);

/** Returns the addressing method of the operand */
int addressingMethod(char *operand);

/** Extracts the register */
int extractRegister(char *operand,char *label, int lineNum);

/** Checks if the first operand legal according to the command */
int isFirstOperandLegal(int addressingMethod, int opcode, int lineNum,char *operand);

/** Checks if the destination operand legal according to the command */
int isSecondOperandLegal(int addressingMethod, int opcode, int lineNum,char *operand);

/** Counts comma to valuate the numbers */
int countComma(char *line);

int isANumber(char* number);