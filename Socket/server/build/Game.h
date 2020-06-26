#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Player.h"

// Welcome part:
void Welcome(FILE* log);
// Complete stuffing and show them:
void PrintShuff(FILE* log, const int Parameters[], PILE* stock);
// Change the ID of the player now
int ChangeID(int ID, int state, int cases, int player);

#endif //GAME_H_INCLUDED