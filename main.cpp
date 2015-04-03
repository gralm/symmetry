#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <time.h>
//#include <Winuser.h>
#include "comm/comm.h"
#include "glutThread.h"
#include "gui/gui.h"

#define UI_UPDATE_TIME          100

using namespace std;

const char g_szClassName[] = "myWindowClass";

int mainMode = 0;


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

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //cout << ".";
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

                changeGuiMode(hwnd, -1, mainMode);

            }
            break;
        case WM_SIZE:
            {
                //HWND hEdit;
                //RECT rcClient;

                //GetClientRect(hwnd, &rcClient);
                //cout << "left, top, right, bottom: " << rcClient.left << ", ";
                //cout << rcClient.top << ", " << rcClient.right << ", " << rcClient.bottom << endl;

                //hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
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
                        break;
                    }
                    case IDC_TEST1_BUTTON: {
                        try1();
                        cout << "radiobutton: " << getSymmetryValue() << endl;
                        break;
                    }
                    case IDC_LISTBOX:{
                        int hiwParam = HIWORD(wParam);
                        // hiwParam = 1 om man vÃ¤ljer ett nytt element
                        // hiwParam = 4 om listboxen blir aktiv
                        // hiwParam = 5 om listboxen bli inaktiv 
                        if (hiwParam != 1)
                        {
                            cout << "nu hÃ¤nde nÃ¥gonting annat Ã¤n ett val i listboxen: " << hiwParam << endl;
                            break;
                        }

                        int chosenIndex = SendDlgItemMessage(hwnd,IDC_LISTBOX,LB_GETCURSEL,0,0);
                        cout << "chosen index i listboxen: " << chosenIndex << endl;
                        CommMsg Chosen(COMM_THREAD_MAIN, COMM_THREAD_GLUT, COMM_MSGTYP_CHOOSE_VERTEX, 0, sizeof(int), (char*) &chosenIndex);
                        commSendMsg(&Chosen);
                       break;}

                    case IDC_NEXT_BUTTON: {
                        if (mainMode == 2) {
                            int chosenSymmetry = getSymmetryValue();
                            cout << "chosen radiobutton: " << chosenSymmetry << endl;
                            CommMsg symmetryMsg(COMM_THREAD_MAIN, COMM_THREAD_GLUT, COMM_MSGTYP_SET_SYMMETRY_VALUE, 0, sizeof(int), (char*) &chosenSymmetry);
                            commSendMsg(&symmetryMsg);
                        }

                        cout << "Det klickas pÃ¥ next" << endl;
                        int newMode = mainMode+1;
                        CommMsg ChangeMode(COMM_THREAD_MAIN, COMM_THREAD_GLUT, COMM_MSGTYP_SET_MODE, 0, sizeof(int), (char*) &newMode);
                        commSendMsg(&ChangeMode);
                        
                        break;
                    }
                    case IDC_PREV_BUTTON:{
                        cout << "Det klickas pÃ¥ prev" << endl;
                        int newMode = mainMode-1;
                        CommMsg ChangeMode(COMM_THREAD_MAIN, COMM_THREAD_GLUT, COMM_MSGTYP_SET_MODE, 0, sizeof(int), (char*) &newMode);
                        commSendMsg(&ChangeMode);
                        break;
                    }
                    default:
                        cout << "icke main button " << endl;
                        break;
                }
                break;    
            }
        case WM_NOTIFY:
            //cout << "NOFITFY: " << "msg: " << msg << "\twParam: " << wParam << "\tlParam: " << lParam <<  endl;
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
    UINT timeToHandleNextThreadEvent = UI_UPDATE_TIME;
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
                    //cout << "MAIN - nu ska mainThreaden dÃ¶das" << endl;
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
                case COMM_MSGTYP_UPDATE_VERTEX: {
                    list<string> returned = stringSplit((const char*) msg.data, msg.dataSiz, ',');
                    
                    list<string>::iterator firstIt = returned.begin();
                    int vertexId = atoi(firstIt->c_str());
                    switch(vertexId)
                    {
                        case VERTEX_CENTERED:
                            *firstIt = "VC";
                            break;
                        case EDGE_CENTERED:
                            *firstIt = "EC";
                            break;
                        case FACE_CENTERED:
                            *firstIt = "FC";
                            break;
                    }

                    //int rowId = vertexId<0? -vertexId-2: vertexId+3;
                    setItemInListView(IDC_VERTICE_LISTVIEW, returned);
                    msg.destroy();
                    break;}
                case COMM_MSGTYP_UPDATE_EDGE:{
                    list<string> returned = stringSplit((const char*) msg.data, msg.dataSiz, ',');
                    setItemInListView(IDC_EDGE_LISTVIEW, returned);
                    msg.destroy();
                    break;}

                case COMM_MSGTYP_UPDATE_FACE:{
                    list<string> returned = stringSplit((const char*) msg.data, msg.dataSiz, ',');
                    
                    cout << "\tREAD HEEEERRE MAIN.cpp" << endl;

                    list<string>::iterator firstIt = returned.begin();
                    firstIt++;
                    firstIt++;
                    firstIt++;

                    int faceType = atoi(firstIt->c_str());
                    switch(faceType)
                    {
                        case NOT_CENTERED:
                            *firstIt = "";
                            break;
                        case VERTEX_CENTERED:
                            *firstIt = "VC";
                            break;
                        case EDGE_CENTERED:
                            *firstIt = "EC";
                            break;
                        case FACE_CENTERED:
                            *firstIt = "FC";
                            break;
                    }


                    for (list<string>::iterator itStr = returned.begin(); itStr != returned.end(); itStr++)
                        cout << "\t" << *itStr << endl;
                    setItemInListView(IDC_FACE_LISTVIEW, returned);
                    msg.destroy();
                    break;}

                case COMM_MSGTYP_SET_MODE: {
                    int newMode = *((int*)(msg.data));
                    cout << "mainMode i GUIt = " << mainMode << endl;

                    changeGuiMode(hwnd, mainMode, newMode);
                    mainMode = newMode;
                    
                    delete msg.data;
                    break;
                }
                case COMM_MSGTYP_ADD_CENTERED_VERTEX:{
                    cout << "Nu addades en centered vertex" << endl;
                    int *val = (int*) msg.data;

                    switch(*val)
                    {
                        case VERTEX_CENTERED:
                            cout << " * blev en vertex-centered vertex" << endl;
                            CheckDlgButton(hwnd, IDC_CHECK_BOX_1, BST_CHECKED);
                            //BST_UNCHECKED
                            break;
                        case EDGE_CENTERED:
                            cout << " * blev en edge-centered vertex" << endl;
                            CheckDlgButton(hwnd, IDC_CHECK_BOX_2, BST_CHECKED);
                            break;
                        case FACE_CENTERED:
                            cout << " * blev en face-centered vertex" << endl;
                            CheckDlgButton(hwnd, IDC_CHECK_BOX_3, BST_CHECKED);
                            break;
                        default:
                            cout << " * blev en vetickevaddetis-centered vertex" << endl;
                            break;
                    }
                    msg.destroy();
                    break;
                    }
                default:
                    cout << "MAIN - nu ska mainThreaden gÃ¶ra nÃ¥got annat" << endl;
                    break;
            }
            // omedelbart efteråt ska det hämtas ytterligare ett meddelande.
            timeToHandleNextThreadEvent = 0;//USER_TIMER_MINIMUM;
            break;

        }
        case COMM_RET_ID_MISSING:
            // om det inte finns nÃ¥gra inkomna meddelanden
            break;
        default:
            cout << "ERR: MAIN - det blev no annat: " << val << endl;
            cout << "ERR: try anyway att killa msg" << endl;
            msg.destroy();
            break;
    }
    if (quitAfter) {
        cout << "kommer man hit?" << endl;
    } else {
        SetTimer(hwnd, 0, timeToHandleNextThreadEvent, checkMainThreads);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    pthread_t glutThread;
    long t = 0;
    if (pthread_create(&glutThread, NULL, glutThreadFunc, (void *)t))
    {
    	cout << "problem med att starta thread" << endl;
    	return 1;
    }

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

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "The title of my window",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 250, 450, NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    SetWindowPos(hwnd, NULL, 100, 100, 500, 600, SWP_NOACTIVATE);
    
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

    int err = pthread_mutex_destroy(&mtxThread);
    if (err)
        printf("Gick snett when destroyya mutexen, err = %d\n", err);

    return Msg.wParam;
}
