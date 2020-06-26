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

PILE ShowAllCards_Player(CARD* head, const char* suit[], const char* rank[], FILE* log, PILE outcard, const int function, const int nowID, SOCKET *sClient)
{
    char message[100];
    CARD *p = head->next;
    // These two dynamic arrays are used to judge the sequence of cards, show in order.
    int *Decision = malloc((p->card.Rank + 1) * sizeof(int)), *num = malloc((p->card.Rank + 1) * sizeof(int));
    // If the player cannot show card, show this/ If only several cards can shown, show them
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
    for (int i = 0; i < head->next->card.Rank; i++, p = head->next){
        for (int j = -1; j < num[i]; j++){
            p = p->next;
        }
        sprintf(message," [%d] %s %s", i + 1, suit[p->card.CardSuit],rank[p->card.Rank]);
        fprintf(log, message);
        send(sClient[nowID - 1], message, strlen(message), 0);
        if (i < head->next->card.Rank -1){
            fprintf(log,",");
        }
    }
    printf("\n");  fprintf(log,"\n"); 

    // If cannot output card, just take card and skip.
    if (cannot == head->next->card.Rank){
        outcard.CardSuit = -1;
        outcard.Rank = -1;
        free(num); free(special);
        return outcard;
    }

    // Input and show the card you want to output. 
    if (special[0] != 0){
            sprintf(message,"\nYou can only play %d card(s):", special[0]);
            _sleep(500);
            send(sClient[nowID - 1], message, strlen(message), 0);
            p = head->next;
            for (int i = 0; i < special[0]; i++){
                for (int j = -1; j < special[i + 1]; j++) p = p->next;
                sprintf(message," [%d] %s %s", i + 1, suit[p->card.CardSuit], rank[p->card.Rank]);
                _sleep(400);
                send(sClient[nowID - 1], message, strlen(message), 0);
                p = head->next;
            }
        p = head->next;
        char card[7] = ""; int cardnum = 0;
        do{
            _sleep(200);
            send(sClient[nowID - 1], "\n", strlen("\n"), 0);
            sprintf(message,"Please input the number of the card you want to play, like 1 2:"); 
            _sleep(500);
            send(sClient[nowID - 1], message, strlen(message), 0);
            _sleep(500);
            recv(sClient[nowID - 1], card, 7, 0);
            cardnum = atoi(card);
        } while (cardnum > special[0] || cardnum < 1);
        for (int j = -1; j < special[cardnum]; j++){
            p = p->next;
        }
        free(special); free(num);
        return p->card;
    }
    char card[7] = ""; int cardnum = 0;
    _sleep(500);
    do {
        p = head->next; 
        _sleep(300);
        send(sClient[nowID - 1], "\n", strlen("\n"), 0);
        sprintf(message,"Please input the number of the card you want to play, like 1 2:"); 
        _sleep(500);
        send(sClient[nowID - 1], message, strlen(message), 0);
        _sleep(500);
        recv(sClient[nowID - 1], card, 7, 0);
        cardnum = atoi(card);
        for (int j = -1; j < num[cardnum - 1]; j++){
            p = p->next;
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