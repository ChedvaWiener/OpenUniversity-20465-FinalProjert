
/* Created by Yehuda Klein on 18/02/2022. */

#ifndef FINALPROJECT_LIST_H
#define FINALPROJECT_LIST_H

typedef struct Node
{
    char* id;
    void* data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct List
{
    Node* head;
    Node* tail;
} List;

#endif /* FINALPROJECT_LIST_H */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../GlobalDefinitions.h"


/** Creates Node with it's own copy of id as a string and a
 *  pointer to data stored */
Node* newNode(char* id, void* data);
/** Frees Node memory, @returns pointer to data that was stored. */
void* deleteNode(Node* nodeToDelete);


/** Create an empty double linked List. */
List* newList();
/** Frees all memory used to store data in list */
void deleteList(List* list);
/** Add node to end of list. */
void addNode(List* list, Node* newNode);
/** Add node to start of list (stack add). */
void stackAddNode(List* list, Node* newNode);
/** @return pointer to Node in list where *(node->id) == *id,
 * NULL if not found. */
Node* listSearch(char* id, List* list);
/** @return pointer to data correlated with ID as a (void*),
 * NULL if not found. */
void* getDataById(char* id, List* list);
/** Delete node from list by ID. Return pointer to Node.data.
 *  @return pointer to data correlated with ID as a (void*),
 *  NULL if Error or not found. */
void* deleteNodeById(char* id, List* list);
/** @return true if list does nat contain ant nodes, false otherwise. */
int isEmpty(List* list);