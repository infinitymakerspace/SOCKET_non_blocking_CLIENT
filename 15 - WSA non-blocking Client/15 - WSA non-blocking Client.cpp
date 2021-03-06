// 15 - WSA non-blocking Client.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#ifndef WM_SOCKET
#define WM_SOCKET WM_USER + 1
#endif // !WM_SOCKET
#ifndef IDT_TIMER1
#define IDT_TIMER1 1
#endif // !IDT_TIMER1
#ifndef IDT_TIMER1_MILIS
#define IDT_TIMER1_MILIS 500
#endif
#include "15 - WSA non-blocking Client.h"

#define MAX_LOADSTRING 100
#define IDC_MAINFRM_BTN_1               40501
// Variables globales:
HINSTANCE hInst;                                // Current Instance
WCHAR szTitle[MAX_LOADSTRING];                  // App Tittle
WCHAR szWindowClass[MAX_LOADSTRING];            // Window name

//*********************COMMON CONTROLS ***************************************************
HWND hwndButton, hwndEdit, hwndStatic, hwndStatic2, hwndMesageRec;
RECT StaticRect1 = { 10,90,80,20 };
RECT StaticRect2 = { 100,90,100,20 };
RECT StaticRect3 = { 10,120,80,20 };
RECT EditRect = { 100,120,0,0 };
RECT ButtonRect = { 100,150,100,20 };
int ID_SendButton = IDC_MAINFRM_BTN_1;

//********************WSA_NON_BLOKING initialization **************************************
WSA_non_blocking_Client WSAnb_Client;

wchar_t IPString[] = L"127.0.0.1";
int portNumber = 27015;
// ******************* FUNCTION DECLARATIONS **********************************************
ATOM                MyRegisterClass();
HWND                InitInstance( int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int Ini_WSA_non_blocking_client(HWND);
void XTrace(LPCTSTR , ...);
void XTrace0(LPCTSTR );
void UpdateUI(HDC , RECT* );
void Ini_UI(HWND);
void ShowMessageControls(BOOL );

/// <summary>
/// App's Entry point
/// </summary>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    hInst = hInstance;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Colocar código aquí.

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY15WSANONBLOCKINGCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass();

    // Realizar la inicialización de la aplicación:
    HWND hwnd = InitInstance ( nCmdShow);
    if (!hwnd) return FALSE;

    if (!Ini_WSA_non_blocking_client(hwnd))return FALSE;
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY15WSANONBLOCKINGCLIENT));

    MSG msg;

    // Bucle principal de mensajes:
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

/// <summary>
/// Initialize Client Socket
/// Initialize TIMER
/// </summary>
/// <param name="hwnd">Main Window Handle</param>
/// <returns>TRUE if succeeds, FalSe if failure. If the failure occurred while
/// initializing the socket, the variable WSAnb_Client..lastWSAError saves the 
/// value returned by WSAGetLastError() </returns>
int Ini_WSA_non_blocking_client(HWND hwnd) {

    if (WSAnb_Client.CreateClientSocket() == FALSE)return FALSE;
    
    if (SetTimer(hwnd,              // handle to main window 
        IDT_TIMER1,             // timer identifier 
        IDT_TIMER1_MILIS,       // interval 
        (TIMERPROC)NULL)       // no timer callback 
        == 0)return FALSE;
    return TRUE;
}
/// <summary>
/// Initialize the User Interface
/// </summary>
/// <param name="hwnd">main Window Handle</param>
void Ini_UI(HWND hwnd) {
    hwndStatic = CreateWindowEx(
        0, L"STATIC",   // predefined class 
        L"Recieve:",         // no window title 
        WS_CHILD  |WS_VISIBLE| ES_LEFT,
        StaticRect1.left,         // x position 
        StaticRect1.top,         // y position 
        StaticRect1.right,        // Button width
        StaticRect1.bottom,        // Button height
        hwnd,         // parent window 
        NULL,           // No menu. 
        hInst,
        NULL);        // pointer not needed 

    hwndMesageRec = CreateWindowEx(
        0, L"STATIC",   // predefined class 
        L"No Message",         // no window title 
        WS_CHILD  | WS_BORDER | ES_LEFT,
        StaticRect2.left,         // x position 
        StaticRect2.top,         // y position 
        StaticRect2.right,        // Button width
        StaticRect2.bottom,        // Button height
        hwnd,         // parent window 
        NULL,           // No menu. 
        hInst,
        NULL);        // pointer not needed 

    hwndStatic2 = CreateWindowEx(
        0, L"STATIC",   // predefined class 
        L"Send:",         // no window title 
        WS_CHILD  | ES_LEFT,
        StaticRect3.left,         // x position 
        StaticRect3.top,         // y position 
        StaticRect3.right,        // Button width
        StaticRect3.bottom,        // Button height
        hwnd,         // parent window 
        NULL,           // No menu. 
        hInst,
        NULL);        // pointer not needed 

    hwndButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"SEND",      // Button text 
        WS_TABSTOP  | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        ButtonRect.left,         // x position 
        ButtonRect.top,         // y position 
        ButtonRect.right,        // Button width
        ButtonRect.bottom,        // Button height
        hwnd,     // Parent window
        (HMENU)IDC_MAINFRM_BTN_1,       // For buttons, hMenu is used to send the WM_BUTTON identifier
        hInst,
        NULL);      // Pointer to identify the button in WndProc()

    hwndEdit = CreateWindowEx(
        0, L"EDIT",   // predefined class 
        NULL,         // no window title 
        WS_CHILD |WS_BORDER| ES_LEFT ,
        0, 0, 0, 0,   // set size in WM_SIZE message 
        hwnd,         // parent window 
        NULL,           // No menu. 
        hInst,
        NULL);        // pointer not needed 

}

/// <summary>
/// Regiter the main window's class
/// </summary>
/// <returns>Returns a class atom that uniquely identifies the class. Zero if fails</returns>
ATOM MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInst;
    wcex.hIcon          = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MY15WSANONBLOCKINGCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY15WSANONBLOCKINGCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

/// <summary>
/// Create and shows the main window.
/// </summary>
/// <param name="nCmdShow">ShowWindow() parameter</param>
/// <returns>Main window handle</returns>
HWND InitInstance( int nCmdShow)
{

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 300, 300, nullptr, nullptr, hInst, nullptr);

   if (!hWnd)
   {
      return 0;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

/// <summary>
/// Main window procedure
/// </summary>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        switch (wParam)
        {
        case IDT_TIMER1:
            XTrace(L"WM_TIMER\n");
            switch (WSAnb_Client.state)
            {
            case WSA_non_blocking_Client::STATE::NONE:
                //wchar_t IPString[] = L"192.168.137.1";
                WSAnb_Client.Attemp_connect(IPString, portNumber);
                break;
            case WSA_non_blocking_Client::STATE::CONNECTED:
                SetWindowTextA(hwndMesageRec, (LPCSTR)WSAnb_Client.BufferRecieved);
                break;
            default:
                break;
            }
             WSAnb_Client.testForEvents(); 

            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    case WM_CREATE :
    {
        Ini_UI(hWnd);
        // Add text to the window. 
        wchar_t text[] = L"Text for EditBox";
        SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)text);
        return 0;
    }
    case WM_COMMAND:
        {
        if (lParam== (LPARAM)hwndButton)
        {
            const int textSize = 1000;
            char text[textSize];
            if(GetWindowTextA(hwndEdit, text, textSize)!=0)
                WSAnb_Client.SendText(text,strlen(text));
            return 0;
        }
            int wmId = LOWORD(wParam);
            // Analizar las selecciones de menú:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                return 0;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                return 0;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_SETFOCUS:
        SetFocus(hwndEdit);
        return 0;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Agregar cualquier código de dibujo que use hDC aquí...
            RECT TextRect = { 10,10,300,100 };
            switch (WSAnb_Client.state)
            {
            case WSA_non_blocking_Client::STATE::NONE:
                DrawText(hdc, L"Estado:None", -1, &TextRect, DT_VCENTER | DT_LEFT);
                ShowMessageControls(FALSE);
                break;
            case WSA_non_blocking_Client::STATE::CONNECTED:
                DrawText(hdc, L"Estado:Conectado", -1, &TextRect, DT_VCENTER | DT_LEFT);
                UpdateUI(hdc, &TextRect);
                ShowMessageControls(TRUE);
                break;
            case WSA_non_blocking_Client::STATE::REQUESTING:
                DrawText(hdc, L"Estado:Solicitando conexion", -1, &TextRect, DT_VCENTER | DT_LEFT);
                UpdateUI(hdc, &TextRect);
                ShowMessageControls(FALSE);
                break;
            default:
                DrawText(hdc, L"Estado:Desconocido", -1, &TextRect, DT_VCENTER | DT_LEFT);
                ShowMessageControls(FALSE);
                break;
            }
            EndPaint(hWnd, &ps);
        }
        break; 
    case WM_SIZE:
    {
        // Make the edit control the size of the window's client area. 
        MoveWindow(hwndMesageRec,
            StaticRect2.left, StaticRect2.top,                    // starting x- and y-coordinates 
            LOWORD(lParam)- StaticRect2.left-20,                // width of client area 
            20,                // height of client area 
            TRUE);                 // repaint window 

        MoveWindow(hwndEdit,
            EditRect.left, EditRect.top,                    // starting x- and y-coordinates 
            LOWORD(lParam)-EditRect.left-20,                // width of client area 
            20,                // height of client area 
            TRUE);                 // repaint window 
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/// <summary>
/// Show the Client's basic parametres ( Status, Ip and Port ) 
/// </summary>
/// <param name="hdc">Device context handler received by BeginPaint () on WM_PAINT</param>
/// <param name="message">Texto para ser mostrado en primer lugar</param>
void UpdateUI(HDC hdc, RECT* TextRect) {
    TextRect->top += 20;
    TextRect->bottom += 20;
    DrawText(hdc, WSAnb_Client.IPString, -1, TextRect, DT_VCENTER | DT_LEFT);
    TextRect->top += 20;
    TextRect->bottom += 20;
    DrawText(hdc, WSAnb_Client.PortString, -1, TextRect, DT_VCENTER | DT_LEFT);

}

/// <summary>
/// Shows or hides the send/recv User Interface
/// </summary>
/// <param name="show">TRUE= show, FALSE = hide</param>
void ShowMessageControls(BOOL show) {
    ShowWindow(hwndStatic, show? SW_SHOW:SW_HIDE);
    ShowWindow(hwndStatic2, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndMesageRec, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndEdit, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndButton, show ? SW_SHOW : SW_HIDE);
}
// Controlador de mensajes del cuadro Acerca de.
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

/// <summary>
/// printf() style debugging
/// https://stackoverflow.com/questions/15240/
/// </summary>
/// <param name="lpszFormat">Debugging text</param>
void XTrace0(LPCTSTR lpszText)
{
    ::OutputDebugString(lpszText);
}

/// <summary>
/// printf() style debugging
/// https://stackoverflow.com/questions/15240/
/// </summary>
/// <param name="lpszFormat">-Debugging text</param>
/// <param name="">.... parameters in _vstprintf_s() style</param>
void XTrace(LPCTSTR lpszFormat, ...)
{
    va_list args;
    va_start(args, lpszFormat);
    int nBuf;
    TCHAR szBuffer[512]; // get rid of this hard-coded buffer
    nBuf = _vstprintf_s(szBuffer, 511, lpszFormat, args);
    ::OutputDebugString(szBuffer);
    va_end(args);
}
