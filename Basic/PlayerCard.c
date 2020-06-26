#include "PlayerCard.h"

CARD* InitialPlayerCard()
{
    // The next of head pointer stone the number of the cards.
    CARD *head = (CARD*)calloc(1,sizeof(CARD)), *CardNum = (CARD*)calloc(1,sizeof(CARD));
    CardNum->card.CardSuit = -1;
    CardNum->card.Rank = 0;
    head->next = CardNum;
    head->prev = NULL;
    CardNum->next = CardNum;
    CardNum->prev = CardNum;
    return head;
}

CARD* Insert_Playercard(int suit, int rank, CARD* head)
{
    CARD* tmp = (CARD*)calloc(1,sizeof(CARD));
    tmp->card.CardSuit = suit;
    tmp->card.Rank = rank;
    tmp->next = head->next;
    tmp->prev = head->next->prev;
    head->next->prev->next = tmp;
    head->next->prev = tmp;
    head->next->card.Rank++;
    return head;
}

PILE ShowAllCards_Player(CARD* head, const char* suit[], const char* rank[], FILE* log, PILE outcard, const int function, const int demo, const int AI_myid, const int nowID)
{
    CARD *p = head->next;
    // These two dynamic arrays are used to judge the sequence of cards, show in order.
    int *Decision = malloc((p->card.Rank + 1) * sizeof(int)), *num = malloc((p->card.Rank + 1) * sizeof(int));
    // If the player cannot show card, show this/ If only one card can shown show this
    int cannot = 0, *special = malloc((p->card.Rank + 1) * sizeof(int)); special[0] = 0;

    // Give the array number for further judgement.
    for (int i = 0; i < head->next->card.Rank; i++){
        p = p->next;
        Decision[i] = p->card.CardSuit * 100 + p->card.Rank;
        num[i] = i;
        // If cannot play, add 1.
        if (p->card.CardSuit != outcard.CardSuit && p->card.Rank != outcard.Rank){
            cannot++;
        } 
        else if (function != 0){ // Special function cards
            if (p->card.Rank == outcard.Rank){special[0]++; special[special[0]] = i; continue;}
            else if (p->card.Rank == 0 || p->card.Rank == 1 || p->card.Rank == 5 || p->card.Rank == 9 || p->card.Rank == 10) {special[0]++; special[special[0]] = i; continue;}
            cannot++;
        }
    }   

    // Give the right order. 
    for (int i = 0; i < head->next->card.Rank; i++){
        for (int j = i - 1; j > -1; j--){
            if (Decision[j] > Decision[j + 1]){
                int tmp = Decision[j], larger = num[j];
                Decision[j] = Decision[j + 1];
                Decision[j + 1] = tmp;
                num[j] = num[j + 1];
                num[j + 1] = larger;
            }
        }
    }   
    free(Decision);

    // Show all the cards on your hand.
    p = head->next;
    if (AI_myid == -1 || AI_myid == nowID){
    for (int i = 0; i < head->next->card.Rank; i++){
            for (int j = -1; j < num[i]; j++){
                p = p->next;
            }
            printf(" [%d] %s %s", i + 1, suit[p->card.CardSuit],rank[p->card.Rank]);
            if (i < head->next->card.Rank -1) printf(",");
            fprintf(log," %s %s", suit[p->card.CardSuit],rank[p->card.Rank]);
            if (i < head->next->card.Rank -1) fprintf(log,",");
            p = head->next;
        }
        printf("\n");  fprintf(log,"\n");
    }else printf("Player %d has %d cards.\n", nowID, head->next->card.Rank);

    // If cannot output card, just take card and skip.
    if (cannot == head->next->card.Rank){
        outcard.CardSuit = -1;
        outcard.Rank = -1;
        free(num); free(special);
        return outcard;
    }

    // Input and show the card you want to output. 
    int cardnum = 0;
    if (special[0] != 0){
        if (AI_myid == -1 || AI_myid == nowID){
            printf("You can only play %d card", special[0]);
            special[0] == 1 ? printf(":") : printf("s:");
            p = head->next;
            for (int i = 0; i < special[0]; i++){
                for (int j = -1; j < special[i + 1]; j++) p = p->next;
                printf(" [%d] %s %s", i + 1, suit[p->card.CardSuit], rank[p->card.Rank]);
                if (i < special[0] - 1) printf(",");
                p = head->next;
            }
        }
        p = head->next;
        do{
            if (demo == 1 || (nowID != AI_myid && AI_myid != -1)) cardnum = 1;
            else { printf("\nThe card you want to play (Input the number of the card, like 1 2):"); scanf("%d",&cardnum);}
        } while (cardnum > special[0] || cardnum < 1);
        for (int j = -1; j < special[cardnum]; j++){
            p = p->next;
        }
        free(special); free(num);
        return p->card;
    }
    do {
        p = head->next;
        if (demo == 1 || (nowID != AI_myid && AI_myid != -1)){
            cardnum ++;
            for (int j = -1; j < num[cardnum - 1]; j++){
                p = p->next;
            }
        }
        else{  
            printf("The card you want to play (Input the number of the card, like 1 2):"); scanf("%d",&cardnum);
            for (int j = -1; j < num[cardnum - 1]; j++){
                p = p->next;
            }
        }
    } while (p->card.CardSuit != outcard.CardSuit && p->card.Rank != outcard.Rank);
    free(special); free(num);
    return p->card;
}

CARD* Remove_Playercard(PILE given, CARD* head)
{
    head->next->card.Rank--;
    CARD *p = head->next, *q;
    while(1){
        p = p->next;
        if (p->card.CardSuit == given.CardSuit && p->card.Rank == given.Rank) break;
    }
    q = p->prev;
    q->next = p->next;
    p->next->prev = q;
    free(p);
    return head;
}

CARD* Remove_PlayercardFirst(CARD* head)
{
    head->next->card.Rank--;
    CARD *p = head->next->next, *q = head->next;
    q->next = p->next;
    p->next->prev = q;
    free(p);
    return head;
}