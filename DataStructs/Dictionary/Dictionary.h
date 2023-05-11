
/* Created by Yehuda Klein on 18/02/2022. */


#include <stdlib.h>
#include <stdio.h>
#include "../List/List.h"
#include "../../GlobalDefinitions.h"

#ifndef FINALPROJECT_DICTIONARY_H
#define FINALPROJECT_DICTIONARY_H

enum errorCodes
{
    SUCCESS, ID_ALREADY_IN_DICT
};

typedef struct Dictionary
{
    int size;
    List** table;
} Dictionary;

#endif /* FINALPROJECT_DICTIONARY_H */



/** Initialize table and initialize all lists to NULL. */
Dictionary* startDictionary(int size);
/** Remove all memory allocated to dictionary and lists. */
int deleteDictionary(Dictionary* dict);

int addDataMember(char* id, void* data, Dictionary* dict);

int hash(char* id, Dictionary* dict);
/** Delete memory used to store data member.
 *  @return A pointer to data held in member being deleted. */
void* deleteDataMemberById(char* id, Dictionary* dict);
/** @return a pointer to data correlated with id */
void* getDataMemberById(char* id, Dictionary* dict);