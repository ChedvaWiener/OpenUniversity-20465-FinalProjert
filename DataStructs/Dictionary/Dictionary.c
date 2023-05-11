
/* Created by Yehuda Klein on 18/02/2022. */


#include "Dictionary.h"

Dictionary* startDictionary(int size)
{
    Dictionary* dict;
    dict = (Dictionary*)malloc(sizeof (Dictionary));
    if (dict == NULL)
    {
        printf("ERROR: On allocating memory to dict.");
        exit(-1);
    }
    dict->size = size;
    dict->table = (List**)calloc(size, sizeof (List*));
    if (dict->table == NULL)
    {
        printf("ERROR: On allocating memory to dict->table.");
        exit(-1);
    }

    return dict;
}

int deleteDictionary(Dictionary* dict)
{
    int i, size = dict->size;
    List* listPointer;
    for (i = 0; i < size; i++)
    {
        listPointer = *(dict->table + i);
        deleteList(listPointer);
    }
    free(dict->table);
    free(dict);

    return 0;
}

int addDataMember(char* id, void* data, Dictionary* dict)
{
    int idHash;
    Node* dataMemNode,* nodeById;
    List** targetListPointer;

    dataMemNode = newNode(id, data);
    idHash = hash(id, dict);

    targetListPointer = ((dict->table) + idHash);
    if (*targetListPointer == NULL)
        *targetListPointer = newList();
    nodeById = listSearch(id, *targetListPointer);
    if (nodeById != NULL)
        return ID_ALREADY_IN_DICT;
    else
        stackAddNode(*targetListPointer, dataMemNode);

    return SUCCESS;
}

int hash(char* id, Dictionary* dict)
{
    long int result = 1;
    int size = dict->size;

    while (*id != '\0')
    {
        result = (result * (*id));
        result = ((result + 1) + (result % 127)) % (size);
        result++;
        id++;
    }
    result -= 1;

    return (int)result;
}

void* deleteDataMemberById(char* id, Dictionary* dict)
{
    int idHash;
    void* data;
    List* targetList;
    List** targetListPointer;
    idHash = hash(id, dict);
    targetListPointer = (dict->table + idHash);
    targetList = *targetListPointer;

    if (targetList == NULL)
        return NULL;

    if(targetList->head == NULL)
    {
        deleteList(targetList);
        *targetListPointer = NULL;
    }

    data = deleteNodeById(id, targetList);

    if(targetList->head == NULL)
    {
        deleteList(targetList);
        *targetListPointer = NULL;
    }

    return data;
}

void* getDataMemberById (char* id, Dictionary* dict)
{
    int idHash;
    List* targetList;
    idHash = hash(id, dict);
    targetList = *(dict->table + idHash);
    if (targetList == NULL)
        return NULL;

    return getDataById(id, targetList);
}

