// BorderlessWindow.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "framework.h"
#include "BorderlessWindow.h"
#include <tlhelp32.h>
#include <shlwapi.h>

#define MAX_LOADSTRING 256

// Globale Variablen:
HINSTANCE hInst;                                // Aktuelle Instanz
WCHAR szTitle[MAX_LOADSTRING];                  // Titelleistentext
HWND g_HWND, g_StartButton, g_InfoLabel, g_HWindowList;
WCHAR szWindowClass[MAX_LOADSTRING];            // Der Klassenname des Hauptfensters.
std::map <DWORD, JDWindow> g_Windows;
HWND g_CurrentActiveBorderless = NULL;
RECT g_CABRect;
LONG_PTR g_CABStyle;

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
#ifdef DEBUG
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

    // TODO: Hier Code einfügen.

    // Globale Zeichenfolgen initialisieren
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BORDERLESSWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Anwendungsinitialisierung ausführen:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BORDERLESSWINDOW));

    MSG msg;

    // Hauptnachrichtenschleife:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BORDERLESSWINDOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BORDERLESSWINDOW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

   g_HWND = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, DEFAULT_SIZE_X, DEFAULT_SIZE_Y, nullptr, nullptr, hInstance, nullptr);

   if (!g_HWND)
   {
      return FALSE;
   }

   ShowWindow(g_HWND, nCmdShow);
   UpdateWindow(g_HWND);

   return TRUE;
}

//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK: Verarbeitet Meldungen für das Hauptfenster.
//
//  WM_COMMAND  - Verarbeiten des Anwendungsmenüs
//  WM_PAINT    - Darstellen des Hauptfensters
//  WM_DESTROY  - Ausgeben einer Beendenmeldung und zurückkehren
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Menüauswahl analysieren:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, About);
            break;
        case IDC_START_PROCEDURE:
        {
            // Somehow State Check for Buttom Press, when Borderless Worked
            if (g_CurrentActiveBorderless)
            {
                SetWindowText(g_StartButton, _T("Start Procedure"));
                if (!ResetWindow(g_CurrentActiveBorderless))
                {
                    MessageBox(hWnd, _T("Handle to Window is Invalid for Resetting"), _T("Warning"), MB_ICONWARNING);
                }
                g_CABRect = {};
                g_CurrentActiveBorderless = NULL;
                break;
            }
            // Do normal Procedure for Borderless here
            TCHAR bufferWindowName[MAX_LOADSTRING];
            if (GetWindowTextLength(g_HWindowList) == 0)
            {
                MessageBox(hWnd, _T("Imput cannot be empty!\n"), _T("Error"), MB_ICONERROR);
                break;
            }
            GetWindowText(g_HWindowList, bufferWindowName, MAX_LOADSTRING);
            HWND window = FindWindowW(NULL, bufferWindowName);
            GetWindowRect(window, &g_CABRect); // Save Rects for resets
            if (!MakeBorderless(window))
            {
                MessageBox(hWnd, _T("Window does not exist anymore!\n"), _T("Error"), MB_ICONERROR);
                g_CABRect = {};
                break;
            }
            print(_T("Finished Borderlessing\n"));
            g_CurrentActiveBorderless = window; // Save last borderlessed Window, for resetting
            SetWindowText(g_StartButton, _T("Reset Window"));
            break;
        }
        case IDM_COMBOBOX:
        {
            if (HIWORD(wParam) == CBN_DROPDOWN) {
                ListWindows(g_Windows);
                UpdateProcessBoxWindow(g_Windows);
            }
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
        break;
    case WM_CREATE:
    {
        CreateMainLayout(hWnd);
        ListWindows(g_Windows);
        UpdateProcessBoxWindow(g_Windows);
        break;
    }
    case WM_SIZE:
    {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        HandleSizeWindow(rect);
    }
    break;
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = DEFAULT_SIZE_X;
        lpMMI->ptMinTrackSize.y = DEFAULT_SIZE_Y;
        //lpMMI->ptMaxTrackSize.x = DEFAULT_SIZE_X;
        lpMMI->ptMaxTrackSize.y = DEFAULT_SIZE_Y;
        break;
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Zeichencode, der hdc verwendet, hier einfügen...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_ABOUT_OK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void UpdateProcessBoxWindow(std::map<DWORD, JDWindow>& pList)
{
    SendMessage(g_HWindowList, CB_RESETCONTENT, 0, 0);
    for (auto window = pList.begin(); window != pList.end(); window++) 
    {
        print(_T("Window Name: %s\n"), window->second.name.c_str());
        SendMessage(g_HWindowList, CB_ADDSTRING, NULL,(LPARAM)window->second.name.c_str());
    }
}

void HandleSizeWindow(RECT& rect)
{
    //print(_T("Right: %d"), rect.right);
    MoveWindow(g_InfoLabel, 15, 15, rect.right - rect.left - 50, 35, TRUE);
    MoveWindow(g_HWindowList,15, 15 + 35 + DEFAULT_MARGIN, rect.right - rect.left - 50, 200, TRUE);

}

BOOL ResetWindow(HWND& p_HWND)
{
    if (!p_HWND || p_HWND == INVALID_HANDLE_VALUE || !GetWindowDC(p_HWND))
        return FALSE;
#ifdef DEBUG
    TCHAR buffer[MAX_LOADSTRING];
    GetWindowText(p_HWND, buffer, MAX_LOADSTRING);
    print(_T("Reset Window: %s\n"), buffer);
#endif // DEBUG
    SetWindowLongPtr(p_HWND, GWL_STYLE, g_CABStyle);
    SetWindowPos(p_HWND, HWND_TOP, g_CABRect.left, g_CABRect.top, g_CABRect.right - g_CABRect.left, g_CABRect.bottom - g_CABRect.top, SWP_SHOWWINDOW);
    p_HWND = NULL;
    return TRUE;
}

BOOL MakeBorderless(HWND& p_HWND)
{
    if (!p_HWND || p_HWND == INVALID_HANDLE_VALUE)
        return FALSE;
#ifdef DEBUG
    TCHAR buffer[MAX_LOADSTRING];
    GetWindowText(p_HWND, buffer, MAX_LOADSTRING);
    print(_T("Make Borderless: %s\n"), buffer);
#endif // DEBUG
    INT width = GetSystemMetrics(SM_CXSCREEN);
    INT height = GetSystemMetrics(SM_CYSCREEN);
    g_CABStyle = GetWindowLongPtr(p_HWND, GWL_STYLE);
    SetWindowLongPtr(p_HWND, GWL_STYLE, WS_VISIBLE | WS_POPUP);
    SetWindowPos(p_HWND, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    return TRUE;
}

void CreateMainLayout(HWND& hWnd) 
{
    g_InfoLabel = CreateWindow(
        _T("Static"),
        _T("Select a windowed Window to make Borderless: "),
        WS_VISIBLE | WS_CHILD,
        15,
        15,
        DEFAULT_SIZE_X - 50,
        35,
        hWnd,
        (HMENU)NULL,
        NULL,
        NULL);

    g_HWindowList = CreateWindow(
        _T("ComboBox"),
        _T(""),
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS | CBS_AUTOHSCROLL | WS_VSCROLL,
        15,
        15 + 35 + DEFAULT_MARGIN,
        DEFAULT_SIZE_X - 50,
        200,
        hWnd,
        (HMENU)IDM_COMBOBOX,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    g_StartButton = CreateWindow(
        _T("Button"),
        _T("Start Procedure"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        15,         // x position 
        60 + 20 + DEFAULT_MARGIN,         // y position 
        120,        // Button width
        25,        // Button height
        hWnd,
        (HMENU)IDC_START_PROCEDURE,
        NULL,
        NULL);
    /*CreateWindow(
        _T("Edit"),
        _T(""),
        WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL ,
        15,
        90 + 20 + DEFAULT_MARGIN,
        DEFAULT_SIZE_X - 50,
        200,
        hWnd,
        (HMENU)NULL,
        NULL,
        NULL);*/
}