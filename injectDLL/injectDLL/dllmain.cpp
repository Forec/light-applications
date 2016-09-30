/*
author: Forec
last edit date: 2016/09/15
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
