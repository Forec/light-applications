#ifndef __SCANNER_H_
#define __SCANNER_H_

#include <WINSOCK2.H>
#include <string>
#include <iostream>
#include <set>
#include <time.h>
#include <process.h>
#pragma comment(lib,"ws2_32.lib")

class Scanner{
private:
	std::set<unsigned int> _openPorts;
	std::string _IP;
	bool _scaned;
	unsigned int _timeout;
	unsigned int _miltimeout;
	unsigned short _core;
	HANDLE hMutex;
	time_t _lastScanTime;
public:
	Scanner(std::string, unsigned int core=8000, 
		unsigned int timeout=2, unsigned int miltimeout = 0);
	bool scan(unsigned int start = 0, unsigned int end = 65535);
	bool is_scaned() { return _scaned; }
	std::set<unsigned int> getOpenPorts(){ return _openPorts; }
	unsigned int getTimeout(){ return _timeout; }
	unsigned int getMilTimeout(){ return _miltimeout; }
	time_t getLastTime() { return _lastScanTime; }
	void insertOpenPorts(unsigned int);
	void printOpenPorts();
};

struct scanParam{
	Scanner *scanner;
	std::string IP;
	unsigned int start;
	unsigned int end;
};

unsigned int __stdcall scanThread(void *);

template<typename T>
T minport(T a, T b){
	return a < b ? a : b;
};

#endif