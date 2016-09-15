/*
author: Forec
create_date: 2016-09-15
email: forec@bupt.edu.cn
*/

// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
	switch (ul_reason_for_call)	{
	case DLL_PROCESS_ATTACH:
		/*
		Start virus here
		WinExec("virus.exe",SW_HIDE);
		*/
		MessageBox(NULL,
			_T("DLL has been injected"),
			_T("Info"),
			MB_ICONINFORMATION);
		break;
	case DLL_THREAD_DETACH:
		MessageBox(NULL,
			_T("DLL has been removed"),
			_T("Info"),
			MB_ICONINFORMATION);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
