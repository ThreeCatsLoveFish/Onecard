#include "Player.h"

PLAYER* Initial_Player()
{
    PLAYER *player_1st = (PLAYER*)calloc(1,sizeof(PLAYER)), *player_2nd = (PLAYER*)calloc(1,sizeof(PLAYER));
    player_1st->ID = 1;
    player_1st->score = 0;
    player_1st->Mycards = InitialPlayerCard();
    player_1st->next = player_2nd;
    player_1st->prev = player_2nd;
    player_2nd->ID = 2;
    player_1st->score = 0;
    player_2nd->Mycards = InitialPlayerCard();
    player_2nd->next = player_1st;
    player_2nd->prev = player_1st;
    return player_1st;
}

PLAYER* Insert_Player(PLAYER *player_1st, int num)
{
    PLAYER *player = (PLAYER*)calloc(1,sizeof(PLAYER));
    player->ID = num;
    player->score = 0;
    player->Mycards = InitialPlayerCard();
    player->next = player_1st;
    player->prev = player_1st->prev;
    player_1st->prev = player_1st->prev->next = player;
    return player_1st;
}

PLAYER* SwitchPlayer(PLAYER *nowplayer, int state)
{
    switch (state)
    {
    case 1:
        nowplayer = nowplayer->next;
        return nowplayer;

    case 0:
        nowplayer = nowplayer->prev;
        return nowplayer;
    
    default:
        break;
    }
}

PLAYER* SkipPlayer(PLAYER *nowplayer, int state)
{
    switch (state)
    {
    case 1:
        nowplayer = nowplayer->next->next;
        return nowplayer;

    case 0:
        nowplayer = nowplayer->prev->prev;
        return nowplayer;
    
    default:
        break;
    }
}