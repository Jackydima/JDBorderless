#pragma once

#include "resource.h"
#include "Process.h"
#include "Window.h"

#define MAX_LOADSTRING 256
#define DEFAULT_MARGIN 10
#define DEFAULT_SIZE_X 400
#define DEFAULT_SIZE_Y 450

/**
* Defined IDCs
*/
#define IDC_START_PROCEDURE 32000
#define IDM_COMBOBOX 32001

void CreateMainLayout(HWND& hWnd);
BOOL MakeBorderless(HWND& p_HWND);
void UpdateProcessBoxWindow(std::map<DWORD, JDWindow>& pList);
void HandleSizeWindow(RECT& rect);
BOOL ResetWindow(HWND& p_HWND);

