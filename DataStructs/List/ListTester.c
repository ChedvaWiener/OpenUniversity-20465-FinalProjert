
/* Created by Yehuda Klein on 18/02/2022. */

#include "ListTester.h"

int testList()
{
/*  int didPassTest = FALSE;
    int rands[RANDS_LENGTH];
    int i, j, idLength;
    char c = 'A';

    char *ids[ID_LENGTH];

    / * Intializes random number generator * /
    time_t t;
    srand((unsigned) time(&t));

    for (i = 0; i < RANDS_LENGTH; i++)
    {
        rands[i] = rand();
    }

    for (i = 0; i < RANDS_LENGTH; i++)
    {
        c = (i % 26);
        ids[i] = (char*)malloc(ID_LENGTH - 1);
        for (j = 0; j < ID_LENGTH - 1; j++)
        {
            *(ids[i] + j) = 'A' + c % 26;
        }

    } */
    int didPass = TRUE;
    int i, *dp;
    List* list;

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

    list = newList();

    for(i = 0; i < 20; i++)
    {
        addNode(list, newNode(ids[i], &(data[i])));
    }
    for(i = 0; i < 20; i++)
    {
        dp = getDataById(ids[19 - i], list);
        printf("%d \n", *dp);
        if (!(*dp == 19 - i))
            didPass = FALSE;
    }
    for(i = 0; i < 20; i++)
    {
        dp = deleteNodeById(ids[19 - i], list);
        printf("%d \n", *dp);
        if (!(*dp == i + 1))
            didPass = FALSE;
    }

    return didPass;
}
