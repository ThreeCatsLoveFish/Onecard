#ifndef PLAYERCARD_H_INCLUDED
#define PLAYERCARD_H_INCLUDED
/* A double circular linked list CARD.
 * The header file "PlayerCard.h" is used to define the struct PlayerCard, 
 * and decline the functions about it.
 * Made by Sun Zhimin on 24 June, 2019.
 */

#include "Card.h"
#include <winsock2.h> 
#include <conio.h>

typedef struct PlayerCard
{
    PILE card;
    struct PlayerCard* prev;
    struct PlayerCard* next;
} CARD;

// Initialize the head pointer of linked lists by dynamic allocation. 
CARD* InitialPlayerCard();        
// Insert a card.
CARD* Insert_Playercard(int suit, int rank, CARD* head);                         
// Remove the card that a player plays.
CARD* Remove_Playercard(PILE given, CARD* head);
// Remove the first card that a player have.
CARD* Remove_PlayercardFirst(CARD* head);
// Show all cards of a player, input the number, judge whether it's okey, output that card.
PILE ShowAllCards_Player(CARD* head, const char* suit[], const char* rank[], FILE* log, PILE outcard, const int function, const int nowID, SOCKET *sClient);    

#endif //PLAYERCARD_H_INCLUDED