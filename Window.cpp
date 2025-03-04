#include "Window.h"

#define MAX_LOADSTRING 256

BOOL CALLBACK EnumWindowsCB(HWND hwnd, LPARAM p_list) {
    WCHAR ptcstr[MAX_LOADSTRING];
    std::map<DWORD, JDWindow>* list = reinterpret_cast<std::map<DWORD, JDWindow>*>((LPVOID)p_list);
    if (!IsWindowVisible(hwnd))
        return TRUE;
    if (!IsWindowEnabled(hwnd))
        return TRUE;
    if (GetWindowTextLength(hwnd) == 0)
        return TRUE;
    GetWindowText(hwnd, ptcstr, MAX_LOADSTRING);
    //print(_T("WindowName: %s\n"), ptcstr);
    list->insert({ (DWORD)hwnd , {hwnd,string(ptcstr)} });
    return TRUE;
}

std::map<DWORD, JDWindow>& ListWindows(std::map<DWORD, JDWindow>& pList)
{
    pList.clear();
	EnumWindows(EnumWindowsCB, (LPARAM)&pList);
    return pList;
}