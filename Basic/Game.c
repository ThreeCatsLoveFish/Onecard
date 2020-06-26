#include "Game.h"

const char *CardSuit[4] = {"Spades","Hearts","Diamonds","Clubs"};
const char *Rank[13] = {"2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace"};

void Game(char *logname, int Parameters[])
{   // Parameters (1 ~ 5): players, cards, decks, round, state (whether auto).
    if (Parameters[4] == 0) system("clear");

    // Open the file.
    FILE *log = fopen(logname,"w+");
    if (log == NULL)  
    {  
        printf("Something is wrong with the log file!\n");
    }

    // Welcome part: offer a chance to play with AI(s)
    Welcome(log);
    int playmode = 1, AI_myid = -1;
    if (Parameters[4] == 0){
        printf("What kind of game do you want? 1. Play with my friends 2. Play with AI(s)\n");
        printf("Your choice is:");
        scanf("%d",&playmode);
        if (playmode == 2) {
            printf("Which ID do you want? From %d to %d: ",1,Parameters[0]);
            scanf("%d",&AI_myid);
        }
    }
    
    // Initial the players:
    PLAYER *FirstPlayer = Initial_Player();
    for (int i = 2; i < Parameters[0]; i++)
    {FirstPlayer = Insert_Player(FirstPlayer, i + 1);}

    // Initial the the stock and discard piles:
    PILE *stock, *discard = Initial_Pile();
    for (int i = 0; i < 52 * Parameters[2]; i++)
    {discard = Insert_Pile(i % 4, i % 13, discard);}
    stock = Shuff_Pile(discard);
    free(discard); discard = Initial_Pile();

    // Complete stuffing:
    PrintShuff(log, Parameters, stock);
    PLAYER* nowplayer = FirstPlayer;
    int nowID = 0, state= 1;

    // Start:
    for (int round = 0; round < Parameters[3]; round++){

        // First, take cards for each player:
        printf("--------------Dealing the cards!--------------\n");
        fprintf(log,"--------------Dealing the cards!--------------\n");
        if (Parameters[4] == 1) printf("----------------------------------------------\n");
        fprintf(log,"----------------------------------------------\n");
        for (int i = 0; i < Parameters[0]; i++){
            for (int j = 0; j < Parameters[1]; j++){
                PILE tmp = ShowAndRemove_Pile(stock);
                stock[0].Rank--;
                if (stock[0].Rank == 0){
                    stock = Shuff_Pile(discard);
                    free(discard); discard = Initial_Pile();
                    // Complete stuffing:
                    PrintShuff(log, Parameters, stock);
                }
                nowplayer->Mycards = Insert_Playercard(tmp.CardSuit, tmp.Rank, nowplayer->Mycards); 
                if (Parameters[4] == 1) printf("Player %d \tSuit:%s      \tRank:%s\n", nowplayer->ID, CardSuit[tmp.CardSuit], Rank[tmp.Rank]);
                fprintf(log,"Player %d \tSuit:%s      \tRank:%s\n", nowplayer->ID, CardSuit[tmp.CardSuit], Rank[tmp.Rank]);
            }
            if (Parameters[4] == 1) printf("----------------------------------------------\n");
            fprintf(log,"----------------------------------------------\n");
            nowplayer = nowplayer->next;
        }
        printf("--------------Dealing  Complete!--------------\n\n");
        fprintf(log,"--------------Dealing  Complete!--------------\n\n");

        /* Decide the player who plays first:
         * Decision is all the value of choice.
         * repeat is to judge whether it's okey, or need another times to choose.
         * state is the clockwise. (1-clockwise/0-opposite)
         * nowID is the ID of the player now.
         */
        if (round == 0) {
            int *Decision = malloc((Parameters[0] + 1) * sizeof(int)), repeat = 0;
            do {printf("-----------Decide the first player!-----------\n");
            fprintf(log,"-----------Decide the first player!-----------\n");
            printf("----------------------------------------------\n");
            fprintf(log,"----------------------------------------------\n");
            repeat = 0;
            Decision[0] = 1;
            for (int i = 0; i < Parameters[0]; i++){
                PILE tmp = ShowAndRemove_Pile(stock);
                stock[0].Rank--;
                discard = Insert_Pile(tmp.CardSuit,tmp.Rank,discard);
                if (stock[0].Rank == 0){
                    stock = Shuff_Pile(discard);
                    free(discard); discard = Initial_Pile();
                    // Complete stuffing:
                    PrintShuff(log, Parameters, stock);
                }
                printf("For player %d, your card is:\n\tSuit:%s      \tRank:%s\n", i + 1, CardSuit[tmp.CardSuit], Rank[tmp.Rank]);
                fprintf(log,"For player %d, your card is:\n\tSuit:%s      \tRank:%s\n", i + 1, CardSuit[tmp.CardSuit], Rank[tmp.Rank]);
                Decision[i + 1] = tmp.CardSuit * 100 + tmp.Rank;
                printf("----------------------------------------------\n");
                fprintf(log,"----------------------------------------------\n");
            }
            for (int i = 1; i < Parameters[0]; i++){
                if (Decision[i] < Decision[i + 1])      Decision[i + 1] = Decision[i];
                else if (Decision[i] > Decision[i + 1]) Decision[0] = i + 1;
                else { repeat = 1; break;}
            }
            } while (repeat == 1);
            nowID = Decision[0];
            free(Decision);
            if (Parameters[4] == 0) system("clear");
            printf("\n-----------Start from the player %d!-----------\n\n", nowID);
            fprintf(log,"\n-----------Start from the player %d!-----------\n\n", nowID);
            for (int i = 0; i < nowID - 1; i++) nowplayer = SwitchPlayer(nowplayer,state);
        }

        if (round > 0){
            // Initial the players:
            state = 1;
            for (int i = 0; i < nowID - 1; i++) nowplayer = SwitchPlayer(nowplayer, state);
        }

        // Ready:
        PILE outcard = ShowAndRemove_Pile(stock), mycard;
        stock[0].Rank--;
        if (stock[0].Rank == 0){
            stock = Shuff_Pile(discard);
            free(discard); discard = Initial_Pile();
            // Complete stuffing:
            PrintShuff(log, Parameters, stock);
        }
        printf("First card:  %s %s\n\n", CardSuit[outcard.CardSuit], Rank[outcard.Rank]);
        fprintf(log,"First card:  %s %s\n\n", CardSuit[outcard.CardSuit], Rank[outcard.Rank]);
        discard = Insert_Pile(outcard.CardSuit, outcard.Rank, discard);
         
        // For the function add up cards: 2,3,7,J,Q
        int FunctionAddup = 0;
        
        while (1){
            // Show the last card:
            if (Parameters[4] == 0) system("clear");
            printf("\nCard on the top: %s %s\n", CardSuit[outcard.CardSuit],Rank[outcard.Rank]);
            fprintf(log,"\nCard on the top: %s %s\n", CardSuit[outcard.CardSuit],Rank[outcard.Rank]);
            // Make sure you are the player nowID.
            int yourchoice = 0;
            do{
                printf("Now ID is: %d\n", nowID);
                if (Parameters[4] == 1 || (nowID != AI_myid && playmode == 2)) yourchoice = nowID;
                else{
                    printf("What is your ID? (Confirm you are the player) ");
                    scanf("%d",&yourchoice);
                }
            } while (yourchoice != nowID);

            // For one player, deal with it.
            if (AI_myid == -1 || AI_myid == nowID) printf("Player %d cards:", nowID);
            fprintf(log,"Player %d cards:", nowID);
            mycard = ShowAllCards_Player(nowplayer->Mycards, CardSuit, Rank, log, outcard, FunctionAddup, Parameters[4], AI_myid, nowID);
            // If cannot output, just take card and skip. 
            if (mycard.CardSuit == -1 && mycard.Rank == -1){
                printf("Player %d cannot play card\n",nowID);
                fprintf(log,"Player %d cannot play card\n",nowID);
                if (FunctionAddup == 0){
                    PILE newcard = ShowAndRemove_Pile(stock);
                    stock[0].Rank--;
                    if (stock[0].Rank == 0){
                        stock = Shuff_Pile(discard);
                        free(discard); discard = Initial_Pile();
                        // Complete stuffing:
                        PrintShuff(log, Parameters, stock);
                    }
                    if (AI_myid == -1 || AI_myid == nowID) printf("Player %d draws: %s %s\n", nowID, CardSuit[newcard.CardSuit], Rank[newcard.Rank]);
                    fprintf(log,"Player %d draws: %s %s\n", nowID, CardSuit[newcard.CardSuit], Rank[newcard.Rank]);
                    nowplayer->Mycards = Insert_Playercard(newcard.CardSuit,newcard.Rank,nowplayer->Mycards);
                }
                else{ // If addup is required.
                    if (AI_myid == -1 || AI_myid == nowID) printf("Player %d draws:", nowID);
                    fprintf(log,"Player %d draws:", nowID);
                    for (int i = 0; i < FunctionAddup; i++){
                        PILE newcard = ShowAndRemove_Pile(stock);
                        stock[0].Rank--;
                        if (stock[0].Rank == 0){
                            stock = Shuff_Pile(discard);
                            free(discard); discard = Initial_Pile();
                            // Complete stuffing:
                            PrintShuff(log, Parameters, stock);
                        }
                        if (AI_myid == -1 || AI_myid == nowID) {
                            printf(" %s %s", CardSuit[newcard.CardSuit], Rank[newcard.Rank]);
                            if (i < FunctionAddup - 1) printf(",");
                        }
                        fprintf(log," %s %s", CardSuit[newcard.CardSuit], Rank[newcard.Rank]);
                        if (i < FunctionAddup - 1) fprintf(log,",");
                        nowplayer->Mycards = Insert_Playercard(newcard.CardSuit,newcard.Rank,nowplayer->Mycards);
                    }
                    FunctionAddup = 0;
                }
            }
            else{
                // Normally output your card.
                outcard = mycard;
                if (outcard.Rank == 0) FunctionAddup += 2; // +2
                else if (outcard.Rank == 1) FunctionAddup += 3; // +3
                else if (outcard.Rank == 5) FunctionAddup = 0; // 7 clear up
                printf("Player %d plays: %s %s\n", nowID, CardSuit[outcard.CardSuit],Rank[outcard.Rank]);
                fprintf(log,"Player %d plays: %s %s\n", nowID, CardSuit[outcard.CardSuit],Rank[outcard.Rank]);
                nowplayer->Mycards = Remove_Playercard(outcard, nowplayer->Mycards);
                discard = Insert_Pile(outcard.CardSuit, outcard.Rank, discard);
            }

            // Only way to get out.
            if (nowplayer->Mycards->next->card.Rank == 0) break;
            else if (mycard.Rank == 9){
                // If jack, skip one player.
                nowplayer = SkipPlayer(nowplayer, state);
                nowID = ChangeID(nowID, state, 2, Parameters[0]);
            }
            else{   
                // If queen, change direction.
                if (mycard.Rank == 10) state = state ? 0 : 1;
                nowplayer = SwitchPlayer(nowplayer, state);
                nowID = ChangeID(nowID, state, 1, Parameters[0]);
            }
        }
        printf("Player %d wins!\n\n--------------------Result--------------------\n", nowID);
        fprintf(log,"Player %d wins!\n\n--------------------Result--------------------\n", nowID);

        // Deduction on scores!
        nowplayer = FirstPlayer;
        for (int i = 0; i < Parameters[0]; i++){
            nowplayer->score -= nowplayer->Mycards->next->card.Rank;
            printf("Player %d: %d \tTotal: %d\n", nowplayer->ID,-1 * nowplayer->Mycards->next->card.Rank,nowplayer->score);
            fprintf(log,"Player %d: %d \tTotal: %d\n", nowplayer->ID,-1 * nowplayer->Mycards->next->card.Rank,nowplayer->score);
            nowplayer = nowplayer->next;
        }
        printf("Round %d ends!\n", round + 1);
        fprintf(log,"Round %d ends!\n", round + 1);

        // Free the cards
        if (round != Parameters[3] - 1){
            PLAYER *p_player = FirstPlayer;
            for (int i = 0; i < Parameters[0]; i++)
            {
                int now = p_player->Mycards->next->card.Rank;
                for (int j = 0; j < now; j++)
                {
                    discard = Insert_Pile(p_player->Mycards->next->next->card.CardSuit, p_player->Mycards->next->next->card.Rank,discard);
                    p_player->Mycards = Remove_PlayercardFirst(p_player->Mycards);
                }
                p_player = p_player->next;
            }
        }
    }

    // Figure out the final winner!
    int *winner = (int*)calloc(Parameters[0],sizeof(int)), people = 0, largescore = nowplayer->score;
    for (int i = 0; i < Parameters[0]; i++){
        if (largescore < nowplayer->next->score) largescore = nowplayer->next->score;
        nowplayer = nowplayer->next;
    }
    for (int i = 0; i < Parameters[0]; i++){
        if (nowplayer->score == largescore){
            winner[people] = nowplayer->ID;
            people++;
        }
        nowplayer = nowplayer->next;
    }
    char champion[48] = "--------------Congratulations!!!!--------------";
    printf("\n");
    for (int i = 0; i < 48; i++) { printf("%c", champion[i]); }    
    printf("\nPlayer ");
    for (int i = 0; i < people; i++) printf("%d ",winner[i]);
    if (people == 1) printf("is the winner!\n");
    else printf("are the winners!\n");
    fprintf(log,"\nCongratulation!!!\n");
    fprintf(log,"\nPlayer ");
    for (int i = 0; i < people; i++) fprintf(log,"%d ",winner[i]);
    if (people == 1) fprintf(log,"is the winner!\n");
    else fprintf(log,"are the winners!\n");
    free(winner);

    // Close the file.
    fclose(log);

    // Free all memory:
    PLAYER *p_player = FirstPlayer, *q_player;
    CARD *p_card, *q_card;
    for (int i = 0; i < Parameters[0]; i++)
    {
        p_card = p_player->Mycards;
        int now = 2 + p_player->Mycards->next->card.Rank;
        for (int j = 0; j < now; j++)
        {
            q_card = p_card->next;            
            free(p_card);
            p_card = q_card;
        }
        q_player = p_player->next;
        free(p_player);
        p_player = q_player;
    }
    free(stock);
    free(discard); 
}

void PrintShuff(FILE* log, const int Parameters[], PILE* stock)
{
    char shuff[48] = ".............Shuffing the cards!...............";
    printf("\n");
    for (int i = 0; i < 48; i++) {printf("%c", shuff[i]);}
    if (Parameters[4] == 1){ 
        for (int i = 0; i < stock[0].Rank; i++)
        {printf("\n\tSuit:%s      \tRank:%s",CardSuit[stock[i+1].CardSuit],Rank[stock[i+1].Rank]);}
    }
    printf("\n-------------Shuffing  Complete!-------------\n\n");
    fprintf(log,"-------------Shuffing the cards!-------------\n");
    for (int i = 0; i < stock[0].Rank; i++)
    {fprintf(log,"\tSuit:%s      \tRank:%s\n",CardSuit[stock[i+1].CardSuit],Rank[stock[i+1].Rank]);}
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