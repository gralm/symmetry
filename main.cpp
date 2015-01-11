#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include "comm/comm.h"
#include "glutThread.h"

#define UI_UPDATE_TIME          100


#define IDC_MAIN_EDIT           101
#define IDC_MAIN_BUTTON         102
#define IDC_TEXT                103
#define IDC_LISTBOX             104
#define IDC_CHECK_BOX_1         105
#define IDC_CHECK_BOX_2         106
#define IDC_CHECK_BOX_3         107
#define IDC_NEXT_BUTTON         180
#define IDC_PREV_BUTTON         181
#define IDC_DEL_BUTTON          182

using namespace std;

const char g_szClassName[] = "myWindowClass";

int mode = 0;


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
    commDestroyMsg();
}


    // http://msdn.microsoft.com/en-us/library/windows/desktop/ms633574(v=vs.85).aspx#system
void createButton(HWND hwnd, const char* text, int left, int width, int top, int height, int id)
{
    // typ = "BUTTON", "EDIT", 

    HFONT hfDefault = NULL;
    if (hfDefault == NULL)
        hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

    HWND hwndOject = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", text, 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        left, top, width, height, hwnd, (HMENU)id, GetModuleHandle(NULL), NULL);
    if(hwndOject == NULL)
        MessageBox(hwnd, "Could not create button.", "Error", MB_OK | MB_ICONERROR);

    SendMessage(hwndOject, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));
}

void createMultiEdit(HWND hwnd, const char* text, int left, int width, int top, int height, int id)
{
    HFONT hfDefault = NULL;
    if (hfDefault == NULL)
        hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

    HWND hwndOject  = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", text, 
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 
        left, top, width, height, hwnd, (HMENU)id, GetModuleHandle(NULL), NULL);

    if(hwndOject == NULL)
        MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

    hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(hwndOject, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
}

void createEdit(HWND hwnd, const char* text, int left, int width, int top, int height, int id)
{
    HFONT hfDefault = NULL;
    if (hfDefault == NULL)
        hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

    HWND hwndOject = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", text, 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        left, top, width, height, hwnd, (HMENU)id, GetModuleHandle(NULL), NULL);
    if(hwndOject == NULL)
        MessageBox(hwnd, "Could not create text field 2.", "Error", MB_OK | MB_ICONERROR);

    SendMessage(hwndOject, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));
}


    // http://www.winprog.org/tutorial/controls.html
    // http://msdn.microsoft.com/en-us/library/windows/desktop/bb775146(v=vs.85).aspx
void createListbox(HWND hwnd, const char* text, int left, int width, int top, int height, int id)
{
    HFONT hfDefault = NULL;
    if (hfDefault == NULL)
        hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

    HWND hwndOject = CreateWindowEx(WS_EX_CLIENTEDGE, "ListBox", text, 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        left, top, width, height, hwnd, (HMENU)id, GetModuleHandle(NULL), NULL);
    if(hwndOject == NULL)
        MessageBox(hwnd, "Could not create text field 2.", "Error", MB_OK | MB_ICONERROR);

    SendMessage(hwndOject, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));

    //SetDlgItemText(hwnd, id, "This is a string");
    /*int index = SendDlgItemMessage(hwnd, id, LB_ADDSTRING, 0, (LPARAM)"Hi B!");
    cout << "aderat index 1: " << index << endl;
    index = SendDlgItemMessage(hwnd, id, LB_ADDSTRING, 2, (LPARAM)"Hi A!");
    cout << "aderat index 2: " << index << endl;
    index = SendDlgItemMessage(hwnd, id, LB_ADDSTRING, 4, (LPARAM)"Hi C!");
    cout << "aderat index 3: " << index << endl;*/
    //ListBox
}

void createCheckbox(HWND hwnd, const char* text, int left, int width, int top, int height, int id) {
    HFONT hfDefault = NULL;
    if (hfDefault == NULL)
        hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
/*      HWND hWndCheckBox = CreateWindowEx(
            0, "Button", "CheckBox",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            200, 200, 100, 20,
            hWndParent, NULL, hInstance, NULL
            );*/

    HWND hWndCheckBox = CreateWindowEx(
            WS_EX_CLIENTEDGE, "Button", text,
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            left, top, width, height,
            hwnd, (HMENU) id, GetModuleHandle(NULL), NULL);
    if(hWndCheckBox == NULL)
        MessageBox(hwnd, "Could not create checkbox", "Error", MB_OK | MB_ICONERROR);

    SendMessage(hWndCheckBox, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));

}


// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {      
        case WM_KEYDOWN: {
            cout << "Key pressed: " << LOWORD(wParam) << ", "
                << HIWORD(wParam) << ", " << LOWORD(lParam) << 
                ", " << HIWORD(lParam) << endl;
                if (LOWORD(wParam) == VK_ESCAPE)   // ESCAPE-key
                    PostQuitMessage(0);

                break;
        }
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
                createMultiEdit(hwnd, "comesing", 0, 200, 0, 100, IDC_MAIN_EDIT);
                createEdit(hwnd, "tjej", 0, 100, 120, 24, IDC_TEXT);
                createButton(hwnd, "hej", 100, 100, 120, 24, IDC_MAIN_BUTTON);
                createCheckbox(hwnd, "vertex-centered", 0, 200, 150, 30, IDC_CHECK_BOX_1);
                createCheckbox(hwnd, "edge-centered", 0, 200, 180, 30, IDC_CHECK_BOX_2);
                createCheckbox(hwnd, "face-centered", 0, 200, 210, 30, IDC_CHECK_BOX_3);
                createListbox(hwnd, "gay", 0, 200, 240, 100, IDC_LISTBOX);
                createButton(hwnd, "taka awayness", 0, 100, 350, 24, IDC_DEL_BUTTON);
                
                createButton(hwnd, "<< Prev", 0, 100, 374, 24, IDC_PREV_BUTTON);
                createButton(hwnd, "Next >>", 100, 100, 374, 24, IDC_NEXT_BUTTON);
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
                //cout << "command: " << LOWORD(wParam) << endl;
                cout << "msg: " << msg << "\twParam: {" << LOWORD(wParam) << ", " << HIWORD(wParam) << "}\tlParam: " << lParam << endl;

                switch(LOWORD(wParam))
                {
                    case IDC_MAIN_BUTTON: {
                        cout << "nu ska det printas here:" << endl;
                        commPrintMsg();

                        //int chosenIndex = SendDlgItemMessage(hwnd,IDC_LISTBOX,LB_SETCURSEL,1,0);
                        //cout << "chosenIndex: " << chosenIndex << endl;
                     
                        break;
                    }
                    case IDC_LISTBOX:{
                        int hiwParam = HIWORD(wParam);
                        // hiwParam = 1 om man väljer ett nytt element
                        // hiwParam = 4 om listboxen blir aktiv
                        // hiwParam = 5 om listboxen bli inaktiv 
                        if (hiwParam != 1)
                        {
                            cout << "nu hände någonting annat än ett val i listboxen: " << hiwParam << endl;
                            break;
                        }

                        int chosenIndex = SendDlgItemMessage(hwnd,IDC_LISTBOX,LB_GETCURSEL,0,0);
                        cout << "chosen index i listboxen: " << chosenIndex << endl;
                        CommMsg Chosen(COMM_THREAD_MAIN, COMM_THREAD_GLUT, COMM_MSGTYP_CHOOSE_VERTEX, 0, sizeof(int), (char*) &chosenIndex);
                        commSendMsg(&Chosen);
                       break;}

                    case IDC_NEXT_BUTTON: {
                        cout << "Det klickas på next" << endl;
                        mode++;
                        CommMsg ChangeMode(COMM_THREAD_MAIN, COMM_THREAD_GLUT, COMM_MSGTYP_SET_MODE, 0, sizeof(int), (char*) &mode);
                        commSendMsg(&ChangeMode);
                        
                        break;
                    }
                    case IDC_PREV_BUTTON:{
                        cout << "Det klickas på prev" << endl;
                        mode--;
                        CommMsg ChangeMode(COMM_THREAD_MAIN, COMM_THREAD_GLUT, COMM_MSGTYP_SET_MODE, 0, sizeof(int), (char*) &mode);
                        commSendMsg(&ChangeMode);
                        break;
                    }
                    default:
                        cout << "icke main button " << endl;
                        break;
                }
                break;    
            }

        /*case WM_TIMER: {
            //cout << "|" << endl;

            //SetTimer(hwnd, 0, UI_UPDATE_TIME, 0);
            }
            break;*/

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
        case COMM_RET_ID_OK: { 
            cout << "MAIN - it was ok" << endl;
            switch(msg.msgTyp)
            {
                case COMM_MSGTYP_EXIT:
                    //cout << "MAIN - nu ska mainThreaden dödas" << endl;
                    quitAfter = true;
                    PostQuitMessage(0);
                    break;
                case COMM_MSGTYP_PAUSE:
                    cout << "MAIN - nu ska mainThreaden pausas" << endl;
                    break;
                case COMM_MSGTYP_CHOOSE_VERTEX:{
                    cout << "MAIN - nu choosas en vertex" << endl;
                    int *index_;
                    index_ = (int*) msg.data;

                    SendDlgItemMessage(hwnd, IDC_LISTBOX, LB_SETCURSEL, *index_, 0);
                    msg.destroy();
                    break;}
                case COMM_MSGTYP_ADD_VERTEX:
                    cout << "MAIN - nu addas en vertex med index: " << endl;
                    cout << "sen blir arrayen: " << msg.data << endl;
                    SendDlgItemMessage(hwnd, IDC_LISTBOX, LB_ADDSTRING, 0, (LPARAM)msg.data);
                    msg.destroy();
                    break;

                case COMM_MSGTYP_ADD_CENTERED_VERTEX:{
                    cout << "Nu addades en centered vertex" << endl;
                    int *val = (int*) msg.data;

                    switch(*val)
                    {
                        case -2:
                            cout << " * blev en vertex-centered vertex" << endl;
                            CheckDlgButton(hwnd, IDC_CHECK_BOX_1, BST_CHECKED);
                            //BST_UNCHECKED
                            break;
                        case -3:
                            cout << " * blev en edge-centered vertex" << endl;
                            CheckDlgButton(hwnd, IDC_CHECK_BOX_2, BST_CHECKED);
                            break;
                        case -4:
                            cout << " * blev en face-centered vertex" << endl;
                            CheckDlgButton(hwnd, IDC_CHECK_BOX_3, BST_CHECKED);
                            break;
                        default:
                            cout << " * blev en vetickevaddetis-centered vertex" << endl;
                            break;
                    }
                    msg.destroy();
                    //delete msg.data;
                    break;
                    }
                default:
                    cout << "MAIN - nu ska mainThreaden göra något annat" << endl;
                    break;
            }
            break;
        }
        case COMM_RET_ID_MISSING:
            // om det inte finns några inkomna meddelanden
            //cout << "MAIN - *" << endl;
            break;
        default:
            cout << "ERR: MAIN - det blev no annat: " << val << endl;
            cout << "ERR: try anyway att killa msg" << endl;
            msg.destroy();
            break;
    }
    if (quitAfter) 
        cout << "kommer man hit?" << endl;
    else 
        SetTimer(hwnd, 0, UI_UPDATE_TIME, checkMainThreads);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int g[8], h;
    h = 0;
    g[0] = 477 + 484;
    g[1] = 283 + 481 + 486;
    g[2] = 170 + 286 + 481 + 283;
    g[3] = 241 + 238;
    g[4] = 242 + 282 + 240 + 171 + 190 + 240;
    g[5] = 240 + 283 + 251 + 285;
    g[6] = 492 + 350;
    g[7] = 284;
    for (int i=0; i<8; i++){
        cout << i << ": " << g[i] << endl;
        h += g[i];
    }
    cout << "sum: " << h << endl;

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
    /*hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "The title of my window",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 250, 450, NULL, NULL, hInstance, NULL);*/
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "The title of my window",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 250, 450, NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    SetWindowPos(hwnd, NULL, 100, 100, 250, 450, SWP_NOACTIVATE);
    
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
