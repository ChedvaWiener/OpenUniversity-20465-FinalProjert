
/* Created by Yehuda Klein on 18/02/2022. */


#include "List.h"

/********* Node methods ******************/
Node* newNode(char *id, void* data)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("ERROR: Failed to allocate memory for newNode in newNode(id, data).");
        exit(-1);
    }

    /* create copy of id in newNode */
    newNode->id = (char*) malloc(strlen(id) + 1);
    if (newNode->id == NULL)
    {
        printf("ERROR: Failed to allocate memory for id in newNode(id, data).");
        exit(-1);
    }
    strcpy(newNode->id,id);

    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

void* deleteNode(Node* nodeToDelete)
{
    void* data = nodeToDelete->data;
    free(nodeToDelete->id);
    free(nodeToDelete);
    return data;
}
/********* end Node methods **************/

List* newList()
{
    List* newList = (List*)malloc(sizeof (List));
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

void deleteList(List* list)
{
    Node* temp;

    if (list == NULL)
        return;

    if(list->head != NULL)
    {
        while(list->head->next != NULL)
        {
            temp = list->head->next->next;
            free(list->head->next->id);
            free(list->head->next);
            list->head->next = temp;
        }
    }

    if(list->head != NULL)
    {
        free(list->head->id);
        free(list->head);
    }
    free(list);
}

void addNode(List* list, Node* newNode)
{
    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
}

void stackAddNode (List* list, Node* newNode)
{
    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->head->prev = newNode;
        newNode->next = list->head;
        list->head = newNode;
    }
}

Node* listSearch(char* id, List* list)
{
    Node* current;

    if(list == NULL)
        return NULL;

    if (list->head == NULL)
        return NULL;

    current = list->head;

    /* while there is a difference (strcmp
     * returns 0 for identical strings) */
    while (current != NULL && strcmp(current->id, id))
        current = current->next;

    return current;
}

void* getDataById(char* id, List* list)
{
    Node* node = listSearch(id, list);

    if (node == NULL)
        return NULL;
    return node->data;
}

void* deleteNodeById(char* id, List* list)
{
    Node* node = listSearch(id, list);
    void* data;

    if(node == NULL)
        return NULL;

    if(list->head == list->tail)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else if(list->head == node)
    {
        list->head = node->next;
        node->next->prev = NULL;
    }
    else if(list->tail == node)
    {
        list->tail = node->prev;
        node->prev->next = NULL;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    data = node->data;
    free(node->id);
    free(node);

    return data;
}


int isEmpty(List* list)
{
    if (list->head == NULL)
        return TRUE;
    else
        return FALSE;
}