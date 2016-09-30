/*
author: Forec
last edit date: 2016/09/25
email: forec@bupt.edu.cn
LICENSE
Copyright (c) 2015-2017, Forec <forec@bupt.edu.cn>

Permission to use, copy, modify, and/or distribute this code for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "scanner.h"

Scanner::Scanner(std::string IP, unsigned int core, 
	unsigned int timeout, unsigned int miltimeout){
	this->_IP = IP;
	this->_timeout = timeout;
	this->_miltimeout = miltimeout;
	if (core < 1)
		this->_core = 1;
	else if (core > 1600)
		this->_core = 1600;
	else
		this->_core = core;
	this->_scaned = false;
	this->_openPorts.clear();
	this->_lastScanTime = 0;
	this->hMutex = CreateMutex(NULL, FALSE, NULL);
}

bool Scanner::scan(unsigned int start, unsigned int end){
	if (end < start || start < 0 || end > 65535){
		std::cout << "Invalid parameters!" << std::endl;
		return false;
	}
	std::cout << "Scan starts..." << std::endl;
	unsigned fragment = (end - start) / this->_core;
	WORD versionRequired = MAKEWORD(1, 1);
	WSADATA wsaData;
	int err = WSAStartup(versionRequired, &wsaData);
	if (err < 0){
		return false;
	}
	HANDLE handles[1600];
	unsigned int i = start;
	unsigned int coreStack = 0;
	while (i <= end){
		scanParam param{ this, this->_IP, i, minport<unsigned int>(i + fragment, end) };
		i += (fragment + 1);
		handles[coreStack++] = (HANDLE)_beginthreadex(NULL, 0,
			scanThread, (void *)(&param), 0, NULL);
		if (handles[coreStack - 1] <= 0){
			coreStack--;
			i -= (fragment + 1);
		}
		//std::cout << coreStack << std::endl;
		Sleep(10);
	}
	std::cout << "Scanning..." << std::endl;
	WaitForMultipleObjects(coreStack, handles, TRUE, INFINITE);//(fragment+4)*1000*(this->_timeout+10));
	for (unsigned int i = 0; i < coreStack; i++){
		CloseHandle(handles[i]);
	}
	WSACleanup();
	this->_scaned = true;
	this->_lastScanTime = time(NULL);
	std::cout << "Scan Finished..." << std::endl;
	return true;
}

void Scanner::printOpenPorts(){
	if (this->_openPorts.size() == 0){
		std::cout << "No port is opening on " << this->_IP << std::endl;
		return;
	}
	std::cout << "Open ports on " << this->_IP << std::endl;
	std::set<unsigned int>::iterator iter;
	for (iter = this->_openPorts.begin(); iter != this->_openPorts.end(); ++iter){
		std::cout << *iter << std::endl;
	}
}

void Scanner::insertOpenPorts(unsigned int port){
	WaitForSingleObject(this->hMutex, INFINITE);
	this->_openPorts.insert(port);
	ReleaseMutex(this->hMutex);
}

unsigned int __stdcall scanThread(void*pM){
	scanParam *param = (scanParam *)pM;
	Scanner * scanner = param->scanner;
	std::string IP = param->IP;
	unsigned int start = param->start;
	unsigned int end = param->end;
	unsigned int timeout = scanner->getTimeout();
	unsigned int miltimeout = scanner->getMilTimeout();
	for (unsigned int port = start; port <= end; port++){
		SOCKADDR_IN scansock_in;
		memset(&scansock_in, 0, sizeof(scansock_in));
		scansock_in.sin_addr.s_addr = inet_addr(IP.c_str());
		scansock_in.sin_family = PF_INET;
		scansock_in.sin_port = htons(port);
		SOCKET scanSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (scanSocket < 0){
			port--;
			continue;
		}
		u_long ul = 1;
		ioctlsocket(scanSocket, FIONBIO, &ul);
		connect(scanSocket, (SOCKADDR*)&scansock_in, sizeof(SOCKADDR));
		/* set timeout */
		fd_set rfd;
		FD_ZERO(&rfd);
		FD_SET(scanSocket, &rfd);
		struct timeval Tout;
		Tout.tv_sec = timeout;
		Tout.tv_usec = miltimeout;
		int ret = select(0, 0, &rfd, 0, &Tout);
		if (ret > 0){
			scanner->insertOpenPorts(port);
		}
		closesocket(scanSocket);
		Sleep(100);
	}
	return 0;
}