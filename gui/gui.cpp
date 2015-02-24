#include "gui.h"

using namespace std;

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
}


void createCheckbox(HWND hwnd, const char* text, int left, int width, int top, int height, int id) {
    HFONT hfDefault = NULL;
    if (hfDefault == NULL)
        hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);


    HWND hWndCheckBox = CreateWindowEx(
            WS_EX_CLIENTEDGE, "Button", text,
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            left, top, width, height,
            hwnd, (HMENU) id, GetModuleHandle(NULL), NULL);
    if(hWndCheckBox == NULL)
        MessageBox(hwnd, "Could not create checkbox", "Error", MB_OK | MB_ICONERROR);

    SendMessage(hWndCheckBox, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));
}

struct ListViewColumn{
	string name;
	int width;
	ListViewColumn(string name_, int width_) 	{name = name_;		width = width_;}
};

void createMultiEdit2(HWND hwnd, const char* text, int left, int width, int top, int height, int id, list<ListViewColumn> columns)
{
    HFONT hfDefault = NULL;
    if (hfDefault == NULL)
        hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

	HWND hwndOject  = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, 
	    	NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 
	        left, top, width, height, hwnd, (HMENU)id, GetModuleHandle(NULL), NULL);
    //    WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 

    LVCOLUMN lvc = { 0 };
    LVITEM   lv  = { 0 };

	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH  | LVCF_FMT;
	lvc.fmt  = LVCFMT_LEFT;

	int i=0;
	for (list<ListViewColumn>::iterator itcp = columns.begin(); itcp != columns.end(); itcp++)
	{
		char tillfalligText[100];
		strcpy(tillfalligText, itcp->name.c_str());
		lvc.cx       = itcp->width;
		lvc.iSubItem = i;
		lvc.pszText  = tillfalligText;
		ListView_InsertColumn(hwndOject, i, &lvc);
		i++;
	}

    if(hwndOject == NULL)
        MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

    hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(hwndOject, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
}

/*void createRadioButtons(HWND hwnd, const char* text, int left, int width, int top, int height, int id) 
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	hWndButton1 = CreateWindowEx(0, "BUTTON", "Button 1", 
		WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 10, 80, 20, hWnd, 
		(HMENU)IDB_RADIO1, hInstance, NULL);

	hWndButton2 = CreateWindowEx(0, "BUTTON", "Button 2", 
		WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 10, 40, 80, 20, hWnd, 
		(HMENU)IDB_RADIO2, hInstance, NULL);
}*/


void changeGuiMode(HWND hwnd, int presentMode, int newMode)
{
	cout << "gamla mode: " << presentMode << "\tnya mode: " << newMode << endl;
	if (presentMode == newMode)
		return;

	list<ListViewColumn> columnNames;
	if (presentMode == -1 && newMode == 0)	// initialize
	{
       // createMultiEdit(hwnd, "comesing", 0, 200, 0, 100, IDC_MAIN_EDIT);
        
        columnNames.push_back(ListViewColumn("id", 30));
		columnNames.push_back(ListViewColumn("x", 55));
		columnNames.push_back(ListViewColumn("y", 55));
		columnNames.push_back(ListViewColumn("z", 55));
		columnNames.push_back(ListViewColumn("m", 55));
        createMultiEdit2(hwnd, "comesing", 0, 250, 0, 150, IDC_VERTICE_LISTVIEW, columnNames);

        columnNames.clear();
        columnNames.push_back(ListViewColumn("id", 50));
		columnNames.push_back(ListViewColumn("fr", 30));
		columnNames.push_back(ListViewColumn("to", 30));
		columnNames.push_back(ListViewColumn("next", 30));
		columnNames.push_back(ListViewColumn("prev", 30));
		columnNames.push_back(ListViewColumn("oppo", 30));
		columnNames.push_back(ListViewColumn("face", 30));
		columnNames.push_back(ListViewColumn("len", 30));
		columnNames.push_back(ListViewColumn("k", 30));
		columnNames.push_back(ListViewColumn("d", 30));
		columnNames.push_back(ListViewColumn("l0", 30));
		columnNames.push_back(ListViewColumn("theta", 30));

        createMultiEdit2(hwnd, "comesing", 0, 380, 150, 100, IDC_EDGE_LISTVIEW, columnNames);

        columnNames.clear();
        columnNames.push_back(ListViewColumn("id", 50));
		columnNames.push_back(ListViewColumn("fr", 30));
		columnNames.push_back(ListViewColumn("y", 30));
		columnNames.push_back(ListViewColumn("z", 30));
        createMultiEdit2(hwnd, "comesing", 200, 200, 0, 100, IDC_EDGE_LISTVIEW, columnNames);


        createEdit(hwnd, "tjej", 250, 100, 50, 24, IDC_TEXT);
        createButton(hwnd, "hej", 350, 100, 50, 24, IDC_MAIN_BUTTON);
        //createCheckbox(hwnd, "vertex-centered", 0, 200, 150, 30, IDC_CHECK_BOX_1);
        //createCheckbox(hwnd, "edge-centered", 0, 200, 180, 30, IDC_CHECK_BOX_2);
        //createCheckbox(hwnd, "face-centered", 0, 200, 210, 30, IDC_CHECK_BOX_3);
        createListbox(hwnd, "gay", 0, 200, 240, 100, IDC_LISTBOX);
        createButton(hwnd, "taka awayness", 0, 100, 350, 24, IDC_DEL_BUTTON);
        
        createButton(hwnd, "<< Prev", 0, 100, 374, 24, IDC_PREV_BUTTON);
        createButton(hwnd, "Next >>", 100, 100, 374, 24, IDC_NEXT_BUTTON);
	} else if (newMode == 1) {
		ShowWindow(GetDlgItem(hwnd, IDC_MAIN_BUTTON), SW_HIDE);
	} else if (newMode == 2) {
		ShowWindow(GetDlgItem(hwnd, IDC_MAIN_BUTTON), SW_SHOW);
	}
}