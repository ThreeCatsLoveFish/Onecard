# Onecard

Author: Zhimin Sun

Course: VG101 2019 SUMMER

Language: C

***

### Introduction to Onecard:
1. What is ONECARD?
    - One Card is a card game played by *n* players over a pre-decided number of rounds *r*, and *d* decks of Poker cards are shuffled, *c* cards are offered to each player. Once everybody has received his cards, the dealer poses a card, face up in front of each player. The game will be played counter-clockwise, starting with the player who received the card with *lowest rank*. The order will be switched as long as anyone plays a Queen. Once the playing order has been decided, all those initial *n* cards are directly put in the discard pile. Finally, the first card of the stock pile is laid face-up to initiate the rank and suit.
2. How to play ONECARD?
    - As the game starts, each player can and only can play *exactly one card*, according to the defined order, either following the rank or the suit defined by the previous card. Any played card directly goes into the discard pile, and anyone who is unable to play should draw a card from the top of the stock pile. If the stock pile is exhausted, the discard pile is shuffled and used as stock pile.
3. How to win the game?
    - As soon as a player has discarded all his cards the round stops. All other players receive a penalty equal to the number of cards left in their hands. The player who won the round initiates the following one, all the other rules remain unchanged. At the end of the r rounds, the final score of each player is determined by summing up of all his penalties. The person with highest score wins. In case of equality, more than one player can be declared winner.
4. What are functions cards?
    - Function cards are the cards that can attack or defense, resulting in drawing several cards or no drawing cards. 2,3 are attacks cards, while 7 is defense card. J,Q can transfer the attack effect to others according to the functions of the card itself.
***

### General description of the project:
1. **Pay attention! This program only allows the user to change the parameters from the terminal argument!** For more detail, you can open this project by using arguments: "Onecard.exe -h".
2. For the argument "-a", you can see the computer playing Onecard with itself, and similarly, you can edit the parameters from the terminal yourself. (This argument is not supported in the Socket part.)
3. For the other arguments, you can see the detail explanation from "Onecard.exe -h", which will offer you a general idea of the usage of all the arguments.
4. For the playing part, you will see the program itself dealing the cards, choosing the first player, shuffling the cards automatically, and you only need to input your ID to prove that you are the player now, and choose the card you want (and can) play, or the computer will keep asking you until you input the card you want to play. *Remember, you can and have to play only one card one time.*
5. If you have no choice but to draw cards, the system will automatically skip your turn and show you the card you deal. Similarly, if some function cards (attack cards) are played before, you are offered the only several cards you can play, and you have to play them.
6. **Remember that you can only input the number and enter for all the parts! (Let alone the Socket part, since you will be asked to input the nickname and IP address of the server.)**
7. You can choose whether to play with your friends in turn or play with AI(s) in the basic version!
8. All the detail of that game will be written in the logfile, whose name is onecard.log if you don't change it.
***

### The position of the documents for the project
- README.md
- ChangeLog.md
- Basic
    - CMakeList.txt (Just cmake!)
    - main.h
    - main.c
    - Card.h
    - Card.c
    - PlayerCard.h
    - PlayerCard.c
    - Player.h
    - Player.c
    - Game.h
    - Game.c
- Socket
    - server
        - Makefile (Just make!)
        - build (.c and .h files about the server are put here!)
            - main.h
            - main.c
            - Card.h
            - Card.c
            - PlayerCard.h
            - PlayerCard.c
            - Player.h
            - Player.c
            - Game.h
            - Game.c
            - server.h
            - server.c
        - Makefile (Just make!)
        - build (.c file about the server is put here!)
            - client.c
***

### Essential structures used:
1. The structure of the cards, including two parts: the suit and the rank. To make sure my datum can be delivered easily, I decide to use number to define both the suit and the rank: from 0 to 3 and from 0 to 12 separately. 
```c
typedef struct card {
    int CardSuit; int Rank;
} PILE;
```
2. The structure of the cards on the players' hands, including three parts: 
    - A structure of the card (containing the information of rank and suit). 
    - A pointer that points to the previous card. 
    - A pointer that points to the next card.
```c
typedef struct PlayerCard {
    PILE card; 
    struct PlayerCard* prev; 
    struct PlayerCard* next;
} CARD;
```
3. The structure of the player, including five parts:
    - A head pointer that points to the unique structure `playercard`.
    - An integer ID that stores that ID of the player.
    - An integer score that stores the score (deduction) of the player.
    - A pointer that points to the previous player.
    - A pointer that points to the next player.
```c
typedef struct Player {
    CARD* Mycards; 
    int ID; int score; 
    struct Player* prev; 
    struct Player* next;
} PLAYER;
```
***

### Functions used:
1. Card.h and Card.c:
    - `Initial_Pile`: Initial the pile of stock and discard, and set the first position as the number of length. 
    - `EnlargeSize_Pile`: If the size is too small, enlarge its size.           
    - `Shuff_Pile`: Remove and shuffle the cards in the discard pile, return the new stock pile.
    - `Insert_Pile`: Inset a card into the pile. (Add the card and add the card number of the pile.)
    - `ShowAndRemove_Pile`: Show the card at the top of pile and remove it.
2. PlayerCard.h and PlayerCard.c:
    - `InitialPlayerCard`: Initialize the head pointer of linked lists by dynamic allocation. 
    - `Insert_Playercard`: Insert a card.
    - `Remove_Playercard`: Remove the card that a player plays.
    - `Remove_PlayercardFirst`: Remove the first card that a player have.
    - `ShowAllCards_Player`: Show all cards of a player, input the number, judge whether it's okay, output that card.
3. Player.h and Player.c:
    - `Initial_Player`: Initial the players whose ID are 1 and 2.
    - `Insert_Player`: Initial a player whose ID is `num`.
    - `SwitchPlayer`: Normally change the player. (If Queen is shown, switch the clockwise.)
    - `SkipPlayer`: If Jack is shown, skip player.
4. Game.h and Game.c:
    - `Welcome`: Welcome part.
    - `PrintShuff`: Complete stuffing and show them.
    - `ChangeID`: Change the ID of the player now.
    - **`Game`: The whole game process. (See "6.Game" for more details.)**
5. main.h and main.c:
    - main: Support the command argument, read the argument, and change the parameters according to the requirement. 
    - help: Print out the help part.
6. Game:
    1. Open the file.
    2. Welcome part: Use function Welcome.
    3. Initial the players.
    4. Initial the the stock and discard piles.
    5. Complete stuffing: Use function `PrintShuff`.
    6. Decide the player who plays first.
    7. Take cards for each player.
    8. Show the last card: for the function add up cards: 2,3,7,J,Q.
    9. Make sure you are the player `nowID`.
    10. If cannot output, just take card and skip. (Include the case that addup is required.)
    11. Normally output your card.
    12. Every time you take a card, judge whether the stock is empty, and take measure if it is true.
    13. Only way to get out the while loop is that one's card is all played.
    14. Deduction on scores!
    15. Free the cards, go into next round.
    16. Figure out the final winner!
    17. Free all memory.
***

### Socket Communication:
1. Open the OneCardServer.exe, and figure out the IP address of the server (that computer). Pay attention that the OneCardServer.exe also allows and only allows you to change the parameters when you open it in terminal, so don't just open it if you want to change the datum!!!
2. Let the players open the OneCardClient.exe one by one, input the IP address of server. (Connect at the same time will not cause a problem.)
3. If there are already two players connect the server, the player will be asked if he/she is prepared one by one, and that offer you a chance to start the game earlier! If you want to connect more players, just one of you input the number 2; else, input the number 1.
4. If the number of the players connected to the server are is the same as the one of the people you set when the server is open, the game starts!
5. The following part, including the input and output, are similar to the basic part, except that they are separately produced on different computers.
6. If the game does not continue, don't worry, wait a moment or press some unnecessary keys to go through it.
7. When the game ends, the result will be shown, you will be told whether you win or lose, and the program will exit!
8. Enjoy yourself! 