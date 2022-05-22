// CirclePopupMenuSample.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CirclePopupMenuSample.h"
#include "STXCirclePopupMenu.h"
#include <set>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

struct CSTXResource
{
    CComPtr<IStream> _imageAdd;
    CComPtr<IStream> _imageUp;
    CComPtr<IStream> _imageMailCircle;
    CComPtr<IStream> _imageSettingCircle;
    CComPtr<IStream> _imagePieCircle;
    CComPtr<IStream> _imageHomeCircle;
    CComPtr<IStream> _imageGeoCircle;
    CComPtr<IStream> _imageClockCircle;
    CComPtr<IStream> _imageChessCircle;
    CComPtr<IStream> _imagePhoneCircle;
    CComPtr<IStream> _imageCalendarCircle;
    CComPtr<IStream> _imageLockCircle;
};

CSTXResource* pResources = NULL;

IStream* LoadImageFromResource(HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType);

IStream* LoadImageFromResource(LPCWSTR lpName, LPCWSTR lpType)
{
    return LoadImageFromResource(GetModuleHandle(NULL), lpName, lpType);
}

IStream* LoadImageFromResource(HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType)
{
    HRSRC hRC = FindResource(hModule, lpName, lpType);
    if (hRC == NULL)
        return NULL;

    HGLOBAL hPkg = LoadResource(hModule, hRC);
    if (hPkg == NULL)
        return NULL;

    DWORD dwSize = SizeofResource(hModule, hRC);
    LPVOID pData = LockResource(hPkg);

    HGLOBAL hImage = GlobalAlloc(GMEM_FIXED, dwSize);
    LPVOID pImageBuf = GlobalLock(hImage);
    memcpy(pImageBuf, pData, dwSize);
    GlobalUnlock(hImage);

    UnlockResource(hPkg);

    IStream* pStream = NULL;
    CreateStreamOnHGlobal(hImage, TRUE, &pStream);

    return pStream;
}

void LoadResources()
{
    pResources = new CSTXResource();

    pResources->_imageAdd = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_ADD), _T("PNG"));
    pResources->_imageUp = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_UP), _T("PNG"));
    pResources->_imageMailCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_MAIL_CIRCLE), _T("PNG"));
    pResources->_imageSettingCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_SETTING_CIRCLE), _T("PNG"));
    pResources->_imagePieCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_PIE_CIRCLE), _T("PNG"));
    pResources->_imageHomeCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_HOME_CIRCLE), _T("PNG"));
    pResources->_imageGeoCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_GEO_CIRCLE), _T("PNG"));
    pResources->_imageClockCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_CLOCK_CIRCLE), _T("PNG"));
    pResources->_imageChessCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_CHESS_CIRCLE), _T("PNG"));
    pResources->_imagePhoneCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_PHONE_CIRCLE), _T("PNG"));
    pResources->_imageCalendarCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_CALENDAR_CIRCLE), _T("PNG"));
    pResources->_imageLockCircle = LoadImageFromResource(MAKEINTRESOURCE(IDB_PNG_LOCK_CIRCLE), _T("PNG"));
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CoInitialize(nullptr);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CIRCLEPOPUPMENUSAMPLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        CoUninitialize();
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIRCLEPOPUPMENUSAMPLE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    CoUninitialize();
    return (int) msg.wParam;
}

std::set<CSTXCirclePopupMenu*> g_floatMenus;
void ShowFloatWindowDemo(HWND hWnd)
{
    for (std::set<CSTXCirclePopupMenu*>::iterator it = g_floatMenus.begin(); it != g_floatMenus.end(); )
    {
        if ((*it)->IsDestroyed())
        {
            delete (*it);
            g_floatMenus.erase(it++); // strict C++03
                               // i = my_set.erase( i ); // more modern, typically accepted as C++03
        }
        else {
            ++it; // do not include ++ i inside for ( )
        }
    }

    if (g_floatMenus.size() > 0)
    {
        return;
    }

    static UINT nFloadWondowIdBase = 0;

    CSTXCirclePopupMenu::RegisterWindowClass();
    CSTXCirclePopupMenu* pNewWindow = new CSTXCirclePopupMenu();
    g_floatMenus.insert(pNewWindow);

    int nSize = 180 + rand() % 180;
    const SIZE windowSize = { nSize, nSize };
    POINT pt;
    GetCursorPos(&pt);
    pNewWindow->Create(_T("FloatingMenu"), WS_POPUP, pt.x - windowSize.cx / 2, pt.y - windowSize.cy / 2, windowSize.cx, windowSize.cy, hWnd, 5000 + nFloadWondowIdBase);
    pNewWindow->SetMenuItemWidthHeight(windowSize.cx / 5);
    pNewWindow->Show();

    nFloadWondowIdBase++;

    pNewWindow->AddMenuItem(pResources->_imageMailCircle, _T("Send Mail"));
    pNewWindow->AddMenuItem(pResources->_imageSettingCircle, _T("Setting"));
    pNewWindow->AddMenuItem(pResources->_imagePieCircle, _T("Pie Chart"));
    pNewWindow->AddMenuItem(pResources->_imageHomeCircle, _T("Home"));
    pNewWindow->AddMenuItem(pResources->_imageGeoCircle, _T("Place"));
    pNewWindow->AddMenuItem(pResources->_imageClockCircle, _T("Time"));
    pNewWindow->AddMenuItem(pResources->_imageChessCircle, _T("Game"));
    pNewWindow->AddMenuItem(pResources->_imagePhoneCircle, _T("My Phone"));
    pNewWindow->AddMenuItem(pResources->_imageCalendarCircle, _T("Calendar"));
    pNewWindow->AddMenuItem(pResources->_imageLockCircle, _T("Lock"));
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIRCLEPOPUPMENUSAMPLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIRCLEPOPUPMENUSAMPLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


   if (!hWnd)
   {
      return FALSE;
   }

   HWND hWndButton = CreateWindow(_T("Button"), _T("Show Popup Menu"), WS_CHILD | WS_VISIBLE, 80, 180, 300, 60, hWnd, (HMENU)1001, nullptr, nullptr);
   if (!hWnd)
   {
       return FALSE;
   }

   LoadResources();
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case 1001:
                ShowFloatWindowDemo(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
