#include "Card.h"

PILE* Initial_Pile()
{
    PILE* head = (PILE*)calloc(1,sizeof(PILE));
    head[0].CardSuit = -1;
    head[0].Rank = 0;
    return head;
}

PILE* EnlargeSize_Pile(PILE* Want)
{
    Want = (PILE*) realloc(Want, (Want[0].Rank + 52 + 1) * sizeof(PILE));
    return Want;
}

PILE* Insert_Pile(int suit, int rank, PILE* head)
{
    if (head[0].Rank % (52) == 0)
    {
        head = EnlargeSize_Pile(head);
    }
    head[0].Rank++;
    head[head[0].Rank].CardSuit = suit;
    head[head[0].Rank].Rank = rank;
    return head;
}

PILE ShowAndRemove_Pile(PILE* head)
{
    return head[head[0].Rank];
}

PILE* Shuff_Pile(PILE* head_discard)
{
    int num = head_discard[0].Rank;
    PILE *shuff = (PILE*)calloc(num, sizeof(PILE));
    for (int i = 0; i < (num); i++)
    {
        shuff[i].CardSuit = head_discard[i + 1].CardSuit;
        shuff[i].Rank = head_discard[i + 1].Rank;
    }
    srand(time(NULL));
    for (int i = 0; i < (num); i++)
    {    
        int case_1 = rand() % (num), case_2 = rand() % (num);
        PILE tmp = {shuff[case_1].CardSuit,shuff[case_1].Rank};
        shuff[case_1].CardSuit = shuff[case_2].CardSuit;
        shuff[case_1].Rank = shuff[case_2].Rank;
        shuff[case_2].CardSuit = tmp.CardSuit;
        shuff[case_2].Rank = tmp.Rank;
    }
    PILE *head_deck = (PILE*)calloc(num + 1, sizeof(PILE));
    head_deck[0].CardSuit = -1;
    head_deck[0].Rank = num;
    for (int i = 0; i < (num); i++)
    {
        head_deck[i + 1].CardSuit = shuff[i].CardSuit;
        head_deck[i + 1].Rank = shuff[i].Rank;
    }
    free(shuff);
    return head_deck;
}