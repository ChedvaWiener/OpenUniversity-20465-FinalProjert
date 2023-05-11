
/* Created by Hedva Wiener on 12/01/2022. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../GlobalDefinitions.h"

#define TRUE 1
#define FALSE 0
#ifndef FINALPROJECT_MACROLIST_H
#define FINALPROJECT_MACROLIST_H

#endif /* FINALPROJECT_MACROLIST_H */

#define SINGLE 1
#define ERROR_MACRO_NOT_FOUND "ERROR: Macro not found\n"
#define ERROR_MACRO_NODE_MALLOC_ERROR "ERROR: When trying to allocate MacroNode in MacroListAdd(char *name, char *body, struct MacroList *list)"
typedef struct MacroNode
{
    char *name;
    char *body;
    struct MacroNode *next;
    struct MacroNode *prev;
} MacroNode;

typedef struct MacroList
{
    struct MacroNode *head;
    struct MacroNode *tail;
} MacroList;

/** Allocates memory for list. */
MacroList *newMacroList();
/** Add a new macro to the end of list. */
void addMacroNode(char *name, char *body, struct MacroList *list);
/** Free allocated memory of deleted node. */
void deleteMacroNode(struct MacroNode *deletedNode, struct MacroList *list);
/** Free all nodes in list and list itself. */
void deleteMacroList(MacroList *list);
/** Returns the body of macro name or ERROR_MACRO_NOT_FOUND if macro is not in list. */
char* findMacro (char *name, MacroList *list);
/** Find given macro in list and print its body. */
void printMacro (char *name, MacroList *list);
/** returns bool value of whether a name is in the macroList */
int doesMacroExistInList(char *name, MacroList *list);