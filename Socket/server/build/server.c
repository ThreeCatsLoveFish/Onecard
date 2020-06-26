#include "server.h"

int server(char *logname, int Parameters[])
{ 
    // Parameters (1 ~ 5): players, cards, decks, round, state (whether auto).
    const char *CardSuit[4] = {"Spades","Hearts","Diamonds","Clubs"};
    const char *Rank[13] = {"2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace"};

    //Initialize WSA 
    WORD sockVersion = MAKEWORD(2,2); 
    WSADATA wsaData; 
    if (WSAStartup(sockVersion, &wsaData) != 0) return 0; 
    
    //Create a listen 
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (slisten == INVALID_SOCKET){ 
        printf("socket error !"); 
        return 0; 
    }
    
    //Fix ID and htons
    SOCKADDR_IN sin; 
    sin.sin_family = AF_INET; 
    sin.sin_port = htons(8888);
    sin.sin_addr.S_un.S_addr = INADDR_ANY; 
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) printf("bind error !");
    
    //Listening
    if (listen(slisten, 5) == SOCKET_ERROR){ 
        printf("listen error !"); 
        return 0; 
    } 
    
    //Continue receiving data
    SOCKET *sClient = (SOCKET*)calloc(Parameters[0], sizeof(SOCKET)); 
    SOCKADDR_IN *remoteAddr = (SOCKADDR_IN*)calloc(Parameters[0],sizeof(SOCKADDR_IN)); 
    int nAddrlen = sizeof(SOCKADDR_IN); 
    char *name = (char*)calloc(70*Parameters[0],sizeof(char)), **nickname = (char**)calloc(Parameters[0],sizeof(char*));
    for (int i = 0; i < Parameters[0]; i++)
    { 
        printf("This is OneCard server, I'm waiting for your order...\n"); 
        sClient[i] = accept(slisten, (SOCKADDR *)(remoteAddr + i), &nAddrlen); 
        if (sClient[i] == INVALID_SOCKET) 
        { 
            printf("accept error !"); 
            continue; 
        } 
        printf("Connected:%s \r\n", inet_ntoa((remoteAddr + i)->sin_addr));

        //Receive Data 
        int ret = recv(sClient[i], name + i * 70, 70, 0); 
        if (ret > 0) for (int j = 0; j < 70; j++) {
            if (name[i * 70 + j] != '\n') printf("%c", name[i * 70 + j]);
            else { name[i * 70 + j + 1] = '\0'; printf("\n"); nickname[i] = &(name[i * 70 + 38]); break;}
        } 
        else printf("Error!");
        

        //Send Data
        char welcome[50]; 
        sprintf(welcome,"Hello, %sThis is OneCard Server! Your ID is %d\n", nickname[i], i + 1);
        send(sClient[i], welcome, strlen(welcome), 0); 

        int start = 0;
        if (i == Parameters[0] - 1) break;
        else if (i != 0){
            char command[100] = "", choose[7];
            sprintf(command,"Please tell me if you are ready, now we have %d players (1 for ready, 2 for not): ", i + 1);
            for (int j = 0; j <= i; j++){
                send(sClient[j], command, strlen(command), 0); 
                recv(sClient[j], choose, 7, 0);
                if (choose[0] == '1') {
                    if (j == i) {start = 1; Parameters[0] = i + 1;}
                }
                else break;  
            }
        }
        if (start == 1) break;
    } 
    
    for (int people = 0; people < Parameters[0]; people++){
        _sleep(500);   
        send(sClient[people],"\nWelcome to OneCard! Game starts!\n",36,0);
        char information[30];    
        sprintf(information,"We have %d player together\n",Parameters[0]);   
        _sleep(500);   
        send(sClient[people],information,30,0);
        for (int people1 = 0; people1 < Parameters[0]; people1++){
            _sleep(500);
            sprintf(information,"player %d: %s", people1 + 1, nickname[people1]);
            send(sClient[people],information,strlen(information),0);
        }
    }

    // Open the file.
    FILE *log = fopen(logname,"w+");
    if (log == NULL)  
    {  
        printf("Something is wrong with the log file!\n");  
    }

    // Welcome part: offer a chance to play with AI(s)
    Welcome(log);
    
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
    int nowID = 1;

    // Start:
    for (int round = 0; round < Parameters[3]; round++){
        int state = 1;

        // First, take cards for each player:
        char dealcard[50];
        sprintf(dealcard,"\n--------------Dealing the cards!--------------\n");
        fprintf(log,dealcard); printf(dealcard);
        for (int people = 0; people < Parameters[0]; people++) send(sClient[people],dealcard,strlen(dealcard),0);
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
                char message[50];
                sprintf(message,"player %d \tSuit:%s      \tRank:%s\n", nowplayer->ID, CardSuit[tmp.CardSuit], Rank[tmp.Rank]);
                send(sClient[i],message,strlen(message),0);
                fprintf(log,"Player %d \tSuit:%s      \tRank:%s\n", nowplayer->ID, CardSuit[tmp.CardSuit], Rank[tmp.Rank]);
            }
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
            int *Decision = malloc((Parameters[0] + 1) * sizeof(int)), repeat;
            char message[70] = "";
            do {
                repeat = 0;          Decision[0] = 1;
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
                    sprintf(message,"-----------Decide the first player!-----------\n");
                    printf(message);     fprintf(log,message);
                    send(sClient[i], message, 70, 0);
                    _sleep(500);
                    sprintf(message,"----------------------------------------------\n");
                    printf(message);     fprintf(log,message);
                    send(sClient[i], message, 70, 0);
                    _sleep(500);
                    sprintf(message,"For player %d, your card is:\n\tSuit:%s      \tRank:%s\n", i + 1, CardSuit[tmp.CardSuit], Rank[tmp.Rank]);
                    printf(message);
                    send(sClient[i], message, 70, 0);
                    _sleep(500);
                    fprintf(log,message);
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
            sprintf(message,"\n-----------Start from the player %d!-----------\n", nowID);
            printf(message);
            for (int people = 0; people < Parameters[0]; people++){
                _sleep(500); send(sClient[people],message,strlen(message),0);
            }
            fprintf(log,message);
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
        char firstcard[40];
        sprintf(firstcard,"First card:  %s %s", CardSuit[outcard.CardSuit], Rank[outcard.Rank]);
        printf(firstcard);
        for (int people = 0; people < Parameters[0]; people++){
            _sleep(500); send(sClient[people],firstcard,strlen(firstcard),0);
        }
        fprintf(log,firstcard);
        discard = Insert_Pile(outcard.CardSuit, outcard.Rank, discard);
         
        // For the function add up cards: 2,3,7,J,Q
        int FunctionAddup = 0;
        
        while (1){
            for (int i = 0; i < Parameters[0]; i++){
                char messages[60];
                sprintf(messages,"\n\nplayer %d: %s\tNumber of Cards: %d", nowplayer->ID, nickname[nowplayer->ID - 1], nowplayer->Mycards->next->card.Rank);
                fprintf(log,messages); printf(messages);
                for (int people = 0; people < Parameters[0]; people++) send(sClient[people],messages,strlen (messages),0);
                nowplayer = nowplayer->next;
            }
            // Show the last card:
            sprintf(firstcard,"\n\nCard on the top: %s %s\n", CardSuit[outcard.CardSuit],Rank[outcard.Rank]);
            printf(firstcard); fprintf(log,firstcard); _sleep(200);
            send(sClient[nowID - 1],firstcard,strlen(firstcard),0);

            // For one player, deal with it.
            char cards[100] = "";
            sprintf(cards,"player %d cards:", nowID);
            fprintf(log,cards); _sleep(10);
            send(sClient[nowID - 1],cards,strlen(cards),0);
            mycard = ShowAllCards_Player(nowplayer->Mycards, CardSuit, Rank, log, outcard, FunctionAddup,  nowID, sClient);
            // If cannot output, just take card and skip. 
            if (mycard.CardSuit == -1 && mycard.Rank == -1){
                sprintf(cards,"\nplayer %d cannot play card\n",nowID);
                fprintf(log,cards); printf(cards);
                _sleep(500);
                for (int people = 0; people < Parameters[0]; people++) send(sClient[people],cards,strlen(cards),0);
                if (FunctionAddup == 0){
                    PILE newcard = ShowAndRemove_Pile(stock);
                    stock[0].Rank--;
                    if (stock[0].Rank == 0){
                        stock = Shuff_Pile(discard);
                        free(discard); discard = Initial_Pile();
                        // Complete stuffing:
                        PrintShuff(log, Parameters, stock);
                    }
                    sprintf(cards,"player %d draws: %s %s", nowID, CardSuit[newcard.CardSuit], Rank[newcard.Rank]);
                    fprintf(log,cards);   _sleep(500);
                    send(sClient[nowID - 1],cards,strlen(cards),0);
                    nowplayer->Mycards = Insert_Playercard(newcard.CardSuit,newcard.Rank,nowplayer->Mycards);
                }
                else{ // If addup is required.
                    sprintf(cards,"player %d draws:", nowID);
                    fprintf(log,cards);
                    _sleep(500);
                    send(sClient[nowID - 1],cards,strlen(cards),0);
                    for (int i = 0; i < FunctionAddup; i++){
                        PILE newcard = ShowAndRemove_Pile(stock);
                        stock[0].Rank--;
                        if (stock[0].Rank == 0){
                            stock = Shuff_Pile(discard);
                            free(discard); discard = Initial_Pile();
                            // Complete stuffing:
                            PrintShuff(log, Parameters, stock);
                        }
                        sprintf(cards," %s %s", CardSuit[newcard.CardSuit], Rank[newcard.Rank]);
                        fprintf(log,cards);  send(sClient[nowID - 1],cards,strlen(cards),0);
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
                sprintf(cards,"\nplayer %d plays: %s %s", nowID, CardSuit[outcard.CardSuit],Rank[outcard.Rank]);
                fprintf(log,cards);  printf(cards);
                for (int people = 0; people < Parameters[0]; people++){
                    _sleep(400); send(sClient[people],cards,strlen(cards),0);
                }
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
        char messages[70];
        sprintf(messages,"\nPlayer %d wins!\n\n--------------------Result--------------------\n", nowID);
        fprintf(log,messages); printf(messages);
        for (int people = 0; people < Parameters[0]; people++) send(sClient[people],messages,strlen(messages),0);

        // Deduction on scores!
        nowplayer = FirstPlayer;
        for (int i = 0; i < Parameters[0]; i++){
            nowplayer->score -= nowplayer->Mycards->next->card.Rank;
            sprintf(messages,"player %d: %sScores: %d \tTotal: %d\n", i + 1, nickname[i],-1 * nowplayer->Mycards->next->card.Rank,nowplayer->score);
            fprintf(log,messages); printf(messages);
            for (int people = 0; people < Parameters[0]; people++) send(sClient[people],messages,strlen(messages),0);
            nowplayer = nowplayer->next;
        }
        sprintf(messages,"Round %d ends!\n", round + 1);
        fprintf(log,messages); printf(messages);
        for (int people = 0; people < Parameters[0]; people++) send(sClient[people],messages,strlen(messages),0);

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
    printf("\nCongratualtion!\n");
    for (int i = 0; i < people; i++) printf("Player %d: %s",winner[i], nickname[winner[i] - 1]);
    if (people == 1) printf("He/She is the winner!\n");
    else printf("They are the winners!\n");
    fprintf(log,"\nCongratulation!!!\n");
    fprintf(log,"\nPlayer ");
    for (int i = 0; i < people; i++) fprintf(log,"%d ",winner[i]);
    if (people == 1) fprintf(log,"is the winner!\n");
    else fprintf(log,"are the winners!\n");
    for (int people = 0, i = 0; people < Parameters[0]; people++){
        int repeat = 0;
        if (people == winner[i] - 1) {_sleep(500); repeat = send(sClient[people],"Game ends! You win!",20,0); i++;}
        else {_sleep(500); send(sClient[people],"Game ends! You lose!",21,0);}
    }
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
    free(stock); free(discard); free(remoteAddr); free(sClient); free(name); free(nickname);
    closesocket(slisten); 
    WSACleanup(); 
    return 0;
}