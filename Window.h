#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "util.h"
#include <map>


struct JDWindow
{
	HWND windowHandle;
	string name;
};


std::map<DWORD, JDWindow>& ListWindows(std::map<DWORD, JDWindow>& pList);
