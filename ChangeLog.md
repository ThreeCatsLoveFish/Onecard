#  ChangeLog
## VG101 2019-SUMMER Project 2: Onecard
Made by Sun Zhimin, last updated on June 29, 2019.
***

##  Series 1.0
### Special version (SOCKET) is added here.
***

### Version 1.0.3
### Made on July 7&8, 2019
#### Fix Bug:
- By fixing the size of the arrays (socket, nickname of player, and so on) at first, instead of just realloc, the problem of messy codes disappears, since '\0' may be replaced by a new data, which is not expected.
- By using the function of _sleep() in the <stdlib.h>, I find that the problem of losting packets is solved, since the network is not so fast, compared with the speed of processing datum in the internal computer, so the function sleep is a must to ensure that the package is sent to the client. 
#### Progress:
- Allow the client to input the nickname, and every player can know the number of cards on other players' hands.

### Version 1.0.2
### Made on July 7, 2019
#### Progress:
- Complete the overall part of server and client, and it's clear that under a same local area network, the socket works well, as long as the IP address is right.
#### Bug:
1. Some contents are lost during the communication, and it's always the same part!
2. Randomly, some messy codes are shown when communication.

### Version 1.0.1
### Made on July 6, 2019
#### Progress:
- Edit the functions of main, Game, PlayerCard.
- Add a c file of server, including the code of server.
- Add a c file of client, including the code of client.
- Basic communication on one computer is realized.

### Version 1.0.0
### Made on July 4, 2019
#### Progress:
1. Understand the basic functions of socket communication, including functions: listen, accept, recv, send.
2. Know how to fix the IP address of the server and the client, including the method to open the socket.

***

##  Series 0.2
##### The process of the whole game (basic part with one computer) is completed here, including the auto demo part.

### Version 0.2.7
### Made on July 2, 2019
#### Progress:
- Offer the player an opportunity to play with AI.

### Version 0.2.6
### Made on June 29, 2019
#### Progress:
- Complete the demo part, the Onecard can be played by the computer itself!
#### Bug:
- The scores are not correct for the second round (or even more).
#### Fix:
- The memory is wrongly processed, and I change the code and it works well now.

### Version 0.2.5
### Made on June 29, 2019
#### Progress:
- Continue writing the progress, following parts are realized:
    -  Add the judge part, and the player's cards special function can be realized.
        -  2 and 3 can add up
        -  7 can cancel the attack
        -  Jack and Queen can defense and transfer the attack
    -  Several functions are added to make it more clear and brief.
        -  Welcome
        -  PrintShuff
        -  ChangeID
#### Bug found:
1.  Sometimes the bug exists when the free function is used.
2.  Sometimes when the special function is shown, and card is not correctly shown, but it can be played correctly.
#### Method:
1.  Initialize the pointer for everytime you change the pointer.
2.  I enlarge the size of dynamic allocation to make sure the space is large enough.

### Version 0.2.4
### Made on June 28, 2019
#### Progress:
- Continue writing the progress, following parts are realized:
    1. Improve the functions in Player.c, including the function ShowAllCards_Player and Remove_Playercard.
    2. Can decide which one plays first.
    3. Take one card as the initial card.
    4. Skip and switch the player can be realized.
    5. Deduction on scores can be realized.

### Version 0.2.3
### Made on June 26, 2019
#### Progress:
- Finally, it came out that the problem occured on the subfunction, since I only express a pointer, and it serves as a copy in the subfunction if I do not return to the outside.
- Continue writing the progress, following parts are realized:
    1. Open a file whose name is the same as the input.
    2. Write the process both in the .exe and .log.
    3. The shuffing part works well.

### Version 0.2.2
### Made on June 25, 2019
#### Progress:
- Create the Game.h and Game.c, in which the whole process of game is dealed here.
- Add the first part of the game, including the welcome part, the shuffing part and the end part. 
#### Bug:
- A big problem occurred when I use malloc and realloc, I cannot understand that problem, since it directly stops my program, but my codes are right.

### Version 0.2.1
### Made on June 24, 2019
#### Progress:
- Create the main.h and main.c, in which the command argument is handled. All the datum will be changed according to the argument inputted.

***

##  Series 0.1
#####  The basic functions are fulfilled here.

### Version 0.1.3
### Made on June 24, 2019
#### Progress:
- Create the Player.h and Player.c, in which I define the struct player, and decline the functions about it.
- Struct:
    ```C
    typedef struct Player
    {
        CARD *Mycards;
        int ID;
        int score;
        struct Player* prev;
        struct Player* next;
    } PLAYER;
    ```
- Functions include:
    - Initial_Player
    - Insert_Player
    - SwitchPlayer
    - SkipPlayer

### Version 0.1.2
### Made on June 24, 2019
#### Progress:
- Create the Playercard.h and Playercard.c, in which I define the struct PlayerCard, and decline the functions about it.
- Struct:
    ```C
    typedef struct PlayerCard
    {
        PILE card;
        struct PlayerCard* prev;
        struct PlayerCard* next;
    } CARD;
    ```
- Functions include:
    - InitialPlayerCard      
    - Insert_Playercard                  
    - Remove_Playercard
    - ShowAllCards_Player  

### Version 0.1.1
### Made on June 24, 2019
#### Progress:
- Create the Card.h and Card.c, in which I define the struct card, and decline the functions about it. 
- Struct:
    ``` C
    typedef struct card
    {
        int CardSuit;
        int Rank;
    } PILE;
    ```
- Functions include: 
    - Initial_Pile
    - EnlargeSize_Pile                   
    - Shuff_Pile
    - Insert_Pile            
    - ShowAndRemove_Pile