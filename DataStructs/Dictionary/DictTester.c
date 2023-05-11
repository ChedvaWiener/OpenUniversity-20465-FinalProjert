
/* Created by Yehuda Klein on 18/02/2022. */


#include "DictTester.h"

char* generateRandomId(int i);

int testDict()
{
    int didPass = TRUE;
    int i, *dp;
    Dictionary *dict;

    int data[20];


    char *ids[20] =
            {
                    "aaaaa",
                    "ididididid",
                    "owfrqowrivoqeirv",
                    "3849hjc9240",
                    "aancuaea",
                    "idi932fhidid",
                    "owfrqo90843hcqeirv",
                    "38403cqwp99240",
                    "a23r343154aa",
                    "ididafefidid",
                    "oerqvoeavwrivoqeirv",
                    "3849eravqev9240",
                    "aanqqgbeuaea",
                    "idi932egfrdid",
                    "owfrrefrrbwrbeqeirv",
                    "38403345240",
                    "3849gqerwfewrhtegrfv9240",
                    "aanqq432653768ea",
                    "idi93dsvcrsvtfynjdid",
                    "owfrrefregfrgwefgqeirv"
            };

    for(i = 0; i < 20; i++)
        data[i] = i;

    dict = startDictionary(1024);

    i = 0;
    for(i = 0; i < 20; i++)
    {
        addDataMember(ids[i], &data[i], dict);
    }
    for(i = 0; i < 20; i++)
    {
        dp = getDataMemberById(ids[i], dict);
        printf("%d", *dp);
    }
    for(i = 0; i < 20; i++)
    {
        dp = deleteDataMemberById(ids[i], dict);
        printf("%d", *dp);
    }

    return didPass;
}

int testDict2()
{
    int dataArr[10000], success = 1, i;
    char* idArr[10000];
    dummy* dummyArr[10000];
    dummy* temp;

    Dictionary* dict = startDictionary(1024);


    time_t t;
    srand((unsigned) time(&t));



    for (i = 0; i < 10000; i++) {
        dataArr[i] = i;
        idArr[i] = generateRandomId(i);
        dummyArr[i] = (dummy*) malloc(sizeof (dummy));
        dummyArr[i]->data = dataArr[i];
        dummyArr[i]->ID = idArr[i];
    }

    for (i = 0; i < 10000; i++) {
        temp = (dummy*)getDataMemberById(idArr[i], dict);
        if(temp == NULL)
        {
            addDataMember(idArr[i], dummyArr[i], dict);
        }
        else
            printf("%d\t%s\n", i, idArr[i]);
    }
    for (i = 0; i < 10000; i++) {
        temp = (dummy *)getDataMemberById(idArr[i], dict);
        if (!(temp->data == i)){
            success = 0;
        }
    }
    for (i = 0; i < 10000; i++) {
        temp = (dummy *) deleteDataMemberById(idArr[i], dict);
        if (!(temp->data == i)){
            success = 0;
        }
    }
    deleteDictionary(dict);

    return success;
}



char* generateRandomId(int i)
{
    int j, temp;
    char * p;
    p = (char*)malloc(5 * sizeof (char));

    for(j = 0; j < 4; j++){
        temp = (rand() % 26) + 'a';
        *p = temp;
        p++;
    }
    *p = '\0';
    p -= 4;

    return p;
}
