#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED
/* A sequence list PILE.
 * The header file "Card.h" is used to define the struct card, 
 * and decline the functions about it.
 * Made by Sun Zhimin on 24 June, 2019.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct card
{
    int CardSuit;
    int Rank;
} PILE;

// Initial and set the first position as the number of length. 
PILE* Initial_Pile();
// If the size is too small, enlarge its size.
PILE* EnlargeSize_Pile(PILE* Want);                             
// Remove and shuff the cards in the deck. 
PILE* Shuff_Pile(PILE* head_discard); 
// Insert a card.           
PILE* Insert_Pile(int suit, int rank, PILE* head);              
// Show the card at the top of pile and remove it.
PILE ShowAndRemove_Pile(PILE* head);

#endif //CARD_H_INCLUDED
