#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include "comm/comm.h"
#include "glutThread.h"

#define IDC_MAIN_EDIT   101
#define IDC_MAIN_BUTTON 102
#define IDC_TEXT        103

#define UI_UPDATE_TIME  100

using namespace std;

const char g_szClassName[] = "myWindowClass";


void printFunc(const char *str, long tid)
{
    int ret = pthread_mutex_lock(&mtxThread);
    printf("Skriver nedan on egen hand, ret = %d\n", ret);
    printf(str, tid);
    pthread_mutex_unlock(&mtxThread);
}

void joinThreads(pthread_t *thread)
{
    //int pthread_join(pthread_t thread, void **value_ptr);
    cout << "trying to joina threaddarna" << endl;

    int commSendMessage(int siz, int id, const void *mem);
    CommMsg exitMsg(COMM_THREAD_MAIN, COMM_THREAD_GLUT, COMM_MSGTYP_EXIT, 0, 0, 0);
    cout << "sending avslut" << endl;
    commSendMsg(&exitMsg);
    cout << "trying to joina threaddarna" << endl;
    int val = pthread_join(*thread, 0);
    cout << "joinat threaddar" << endl;
    switch(val)
    {
        case 0:
            cout << "det gick bra att joina threadsen" << endl;
            break;
        default:
            cout << "Ett fel blev det here: " << val << endl;
    }
    commDestroyMessages();
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {      
        case WM_LBUTTONDOWN: {
                cout << "L";
                char szFileName[MAX_PATH];
                HINSTANCE hInstance = GetModuleHandle(NULL);

                GetModuleFileName(hInstance, szFileName, MAX_PATH);
                cout << MAX_PATH;
                MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);
            }
            break; 
        case WM_CREATE: {
                HFONT hfDefault;
                HWND hEdit;

                hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", 
                    WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 
                    0, 0, 100, 100, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);


                if(hEdit == NULL)
                    MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

                hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
                SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));



                HWND hWndButton = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", "OK", 
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    50, 120, 50, 24, hwnd, (HMENU)IDC_MAIN_BUTTON, GetModuleHandle(NULL), NULL);
                if(hWndButton == NULL)
                    MessageBox(hwnd, "Could not create button.", "Error", MB_OK | MB_ICONERROR);

                SendMessage(hWndButton, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));


                HWND hWndText = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "OK", 
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    0, 120, 50, 24, hwnd, (HMENU)IDC_TEXT, GetModuleHandle(NULL), NULL);
                if(hWndText == NULL)
                    MessageBox(hwnd, "Could not create text field 2.", "Error", MB_OK | MB_ICONERROR);

                SendMessage(hWndText, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));


            }
            break;
        case WM_SIZE:
            {
                HWND hEdit;
                RECT rcClient;

                GetClientRect(hwnd, &rcClient);
                //cout << "left, top, right, bottom: " << rcClient.left << ", ";
                //cout << rcClient.top << ", " << rcClient.right << ", " << rcClient.bottom << endl;

                hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
                //SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
            }
            break;
        case WM_COMMAND:
            {
                cout << "command: " << LOWORD(wParam) << endl;
                switch(LOWORD(wParam))
                {
                    case IDC_MAIN_BUTTON: {
                        char hej[] = "Mattias Alm heter jag";
                        commSendMessage(sizeof(hej), 13, hej);
                        break;
                    }
                    default:
                        cout << "icke main button " << endl;
                        break;
                }
                break;    
            }

        case WM_TIMER: {
            //cout << "|" << endl;

            //SetTimer(hwnd, 0, UI_UPDATE_TIME, 0);
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void CALLBACK checkMainThreads(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    //cout << "|" << endl;
    bool quitAfter = false;
    static CommMsg msg;
    int val = commGetMsg(COMM_THREAD_MAIN, &msg);

    switch(val)
    {
        case COMM_ID_OK: { 
            cout << "MAIN - it was ok" << endl;
            switch(msg.msgTyp)
            {
                case COMM_MSGTYP_EXIT:
                    cout << "MAIN - nu ska mainThreaden dödas" << endl;
                    quitAfter = true;
                    PostQuitMessage(0);
                    break;
                case COMM_MSGTYP_PAUSE:
                    cout << "MAIN - nu ska mainThreaden pausas" << endl;
                    break;
                default:
                    cout << "MAIN - nu ska mainThreaden göra något annat" << endl;
                    break;
            }
            break;
        }
        case COMM_ID_MISSING:
            // om det inte finns några inkomna meddelanden
            //cout << "MAIN - *" << endl;
            break;
        default:
            cout << "MAIN - det blev no annat: " << val << endl;
            break;
    }
    if (quitAfter) 
        cout << "kommer man hit?" << endl;
    else 
        SetTimer(hwnd, 0, UI_UPDATE_TIME, checkMainThreads);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    pthread_t glutThread;
    long t;
    int rc = pthread_create(&glutThread, NULL, glutThreadFunc, (void *)t);

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "The title of my window",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 150, 200, NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    
    SetTimer(hwnd, 0, UI_UPDATE_TIME, checkMainThreads);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    joinThreads(&glutThread);

    cout << "will destroyya mutexen " << endl;
    int err = pthread_mutex_destroy(&mtxThread);
    if (err)
        printf("Gick snett when destroyya mutexen, err = %d\n", err);
    else 
        printf("Det gick ok att kloosa mutexen\n");

    return Msg.wParam;
}
