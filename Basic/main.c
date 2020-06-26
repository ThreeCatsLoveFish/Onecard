#include "main.h"

int main(int argc,char *argv[]){
    // Parameters (1 ~ 5): players, cards, decks, round, state (whether auto).
    int Parameters[5] = {4,5,2,1,0}; 
    char logname[32] = "onecard.log";
    
    // Judge the parameters!
    for (int i = 1; i < argc; i++){   
        if ((argv[i][0])!='-'){
            printf("You input the wrong parameter!");
            return 1;
        }
        switch (*(argv[i] + 1)){
            case 'h':
                help();
                return 0;
            case '-':
                if( *(argv[i] + 2)=='l' && *(argv[i] + 3)=='o' && *(argv[i] + 4)=='g' )  
                { i++; sprintf(logname,"%s",argv[i]); } 
                break;
            case 'n': 
                Parameters[0] = atoi(argv[i + 1]);  i++;
                break;
            case 'c': 
                Parameters[1] = atoi(argv[i + 1]);  i++;
                break;
            case 'd': 
                Parameters[2] = atoi(argv[i + 1]);  i++;
                break;
            case 'r':
                Parameters[3] = atoi(argv[i + 1]);  i++;
                break;
            case 'a':
                Parameters[4] = 1;
                break;
        }
    }

    // Start!
    Game(logname, Parameters);
}

void help(){
    printf("************************************Welcome to OneCard!*****************************************\n");
    printf("-h|    --help             Print this help message\n");
    printf("--log  filename|          Write the logs in filename                      (default: onecard.log)\n");
    printf("-n n|  --player-number=n  n players, n must be larger than 2              (default: 4)\n");
    printf("-c c|  --initial-cards=c  Deal c cards per player, c must be at least 2   (default: 5)\n");
    printf("-d d|  --decks=d          Use d decks 52 cards each, d must be at least 2 (default: 2)\n");
    printf("-r r|  --rounds=r         Play r rounds, r must be at least 1             (default: 1)\n");
    printf("-a|    --auto             Run in demo mod\n");
    printf("************************************************************************************************\n");
}