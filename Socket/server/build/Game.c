#include "Game.h"

void PrintShuff(FILE* log, const int Parameters[], PILE* stock)
{
    char *CardSuits[4] = {"Spades","Hearts","Diamonds","Clubs"};
    char *Ranks[13] = {"2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace"};
    char shuff[48] = ".............Shuffing the cards!...............";
    printf("\n");
    for (int i = 0; i < 48; i++) {printf("%c", shuff[i]);}
    printf("\n-------------Shuffing  Complete!-------------\n\n");
    fprintf(log,"-------------Shuffing the cards!-------------\n");
    for (int i = 0; i < stock[0].Rank; i++)
    {fprintf(log,"\tSuit:%s      \tRank:%s\n",CardSuits[stock[i+1].CardSuit],Ranks[stock[i+1].Rank]);}
    fprintf(log,"-------------Shuffing  Complete!-------------\n\n");
}

void Welcome(FILE* log)
{
    fprintf(log,"---------------------------------------------\n");
    fprintf(log,"-------------Welcome to Onecard!-------------\n");
    fprintf(log,"---------------------------------------------\n\n");
    char welcome[46] = "*************Welcome to Onecard!**************";
    for (int i = 0; i < 46; i++) {printf("%c",welcome[i]);}
    printf("\n\n");
}

int ChangeID(int ID, int state, int cases, int player)
{
    switch (cases)
    {
    case 2: // Jack
        ID = state ? ID + 2 : ID - 2;
        if (ID > player) ID -= player;
        else if (ID < 1) ID += player;
        break;
    case 1: // Normal
    default:
        ID = state ? ID + 1 : ID - 1;
        if (ID > player) ID -= player;
        else if (ID < 1) ID += player;
        break;
    }
    return ID;
}