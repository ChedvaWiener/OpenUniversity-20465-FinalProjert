
/* Created by Hedva Wiener on 12/01/2022. */

#include "MacroList.h"


MacroList *newMacroList()
{
    MacroList *newList = (struct MacroList*)calloc(SINGLE, sizeof(MacroList));
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}
void addMacroNode(char *name, char *body, MacroList *list)
{
    struct MacroNode *newNode;
    newNode = (struct MacroNode*)calloc(SINGLE,sizeof(struct MacroNode));
    if (!newNode)
    {
        printf("%s\n", ERROR_MACRO_NODE_MALLOC_ERROR);
        exit(-1);
    }
    /* copy macro name */
    newNode->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newNode->name,name);
    /* copy macro body */
    newNode->body = (char*) malloc((strlen(body) + 1) * sizeof(char));
    strcpy(newNode->body,body);

    newNode->next = NULL;
    if(list->head == NULL)
    {
        list->head = newNode;
        newNode->prev = NULL;
    }
    else
    {
        list->tail->next = newNode;
        newNode->prev = list->tail;
    }
    list->tail = newNode;
}

void deleteMacroNode(MacroNode *deletedNode, MacroList *list)
{
    if(list->head == deletedNode)
    {
       list->head = list->head->next;
    }
    else if(list->tail == deletedNode)
    {
        list->tail = list->tail->prev;
    }
    else
    {
        deletedNode->prev->next = deletedNode->next;
        deletedNode->next->prev = deletedNode->prev;
    }
    free(deletedNode->name); /* free space of macro name */
    free(deletedNode->body); /* free space of macro body */
    free(deletedNode);
}

void deleteMacroList(MacroList *list)
{
    struct MacroNode *temp;
    temp = list->head;
    while(temp != list->tail)
    {
        temp = temp->next;
        /* delete previous node */
        free(temp->prev->name); /* free space of macro name */
        free(temp->prev->body); /* free space of macro body */
        free(temp->prev);
    }
}

void printMacro (char *name, MacroList *list)
{
    printf("%s\n", findMacro(name, list));
}

char* findMacro (char *name, MacroList *list)
{
    MacroNode *current = list->head;
    while(current && strcmp(current->name, name) != 0)
    {
        current = current->next;
    }
    return current ? (current->body) : ERROR_MACRO_NOT_FOUND;
}

int doesMacroExistInList(char *name, MacroList *list)
{
    MacroNode *current = list->head;
    while(current && strcmp(current->name, name) != 0)
    {
        current = current->next;
    }
    return current ? TRUE : FALSE;
}