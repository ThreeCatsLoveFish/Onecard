#include <stdio.h> 
#include <string.h>
#include <winsock2.h> 

#pragma  comment(lib,"ws2_32.lib") 

int main(int argc, char* argv[])
{ 
    WORD sockVersion = MAKEWORD(2,2); 
    WSADATA data; 
    if(WSAStartup(sockVersion, &data) != 0) 
    { 
        return 0; 
    } 
    SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if(sclient == INVALID_SOCKET) 
    { 
        printf("invalid socket!"); 
        return 0; 
    } 

    SOCKADDR_IN serAddr; 
    serAddr.sin_family = AF_INET; 
    serAddr.sin_port = htons(8888); 
    char address[20] = "";  printf("Please input the IP of the server:"); scanf("%s",address);
    serAddr.sin_addr.S_un.S_addr = inet_addr(address);
    if (connect(sclient, (SOCKADDR *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) 
    { 
        printf("Connect error!\n"); 
        system("pause");
        closesocket(sclient); 
        return 0; 
    } 

    printf("Please input your name (no longer than 20 characters in English):");
    char information[65], name[21], recData[255]; 
    scanf("%s",name);
    sprintf(information, "Hello, OneCard Server, this is player %s\n",name);
    send(sclient, information, strlen(information), 0); 
    while (1){
        int ret = recv(sclient, recData, 255, 0); 
        if (ret > 0){ 
            recData[ret] = 0x00; 
            printf(recData); 
            if (recData[0] == 'P'){
                char choose[7] = ""; scanf("%s",choose); getchar();
                send(sclient, choose, 7, 0); 
            } else if (recData[0] == 'G'){
                _sleep(5000);
                break;
            }
        } 
    }
    closesocket(sclient); 
    WSACleanup(); 
    return 0; 
}

