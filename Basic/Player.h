#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
/* A double circular linked list PLAYER.
 * The header file "Player.h" is used to define the struct player, 
 * and decline the functions about it.
 * Made by Sun Zhimin on 24 June, 2019.
 */

#include "PlayerCard.h"

typedef struct Player
{
    CARD *Mycards;
    int ID;
    int score;
    struct Player* prev;
    struct Player* next;
} PLAYER;

// Initial the player whose ID is 1.
PLAYER* Initial_Player();
// Initial a player whose ID is num.
PLAYER* Insert_Player(PLAYER *firstplayer, int num);
// If Queen is shown, switch the clockwise.
PLAYER* SwitchPlayer(PLAYER *nowplayer, int state);
// If Jack is shown, skip player.
PLAYER* SkipPlayer(PLAYER *nowplayer, int state);

#endif //PLAYER_H_INCLUDED
