#include "gui.h"

using namespace std;

HWND hwndVertexListView = 0;
HWND hwndEdgeListView = 0;
HWND hwndFaceListView = 0;

int numOfVerticeRows = 0;
int numOfEdgeRows = 0;
int numOfFaceRows = 0;

list<string> stringSplit(const char *str, int len, char splitChar)
{
	list<string> strList;
	if (len == 0)
		return strList;

	string nyString= "";
	for (const char *c=str; c < str+len; c++)
	{
		if (*c == splitChar)
		{
			strList.push_back(nyString);
			nyString = "";
		} else 
			nyString += *c;
	}
	strList.push_back(nyString);
	return strList;
}

list<string> stringSplit(string str, char splitChar)
{
	return stringSplit(str.c_str(), str.size(), splitChar);
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

HWND createListView(HWND hwnd, const char* text, int left, int width, int top, int height, int id, list<ListViewColumn> columns)
{
    HFONT hfDefault = NULL;
    if (hfDefault == NULL)
        hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

	HWND hwndObject  = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, 
	    	NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 
	        left, top, width, height, hwnd, (HMENU)id, GetModuleHandle(NULL), NULL);
    //    WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 

	// id = _In_opt_  HINSTANCE hInstance
	// hwndObject = HWND WINAPI 

    LVCOLUMN lvc = { 0 };
    //LVITEM   lv  = { 0 };

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
		ListView_InsertColumn(hwndObject, i, &lvc);
		i++;
	}

    if(hwndObject == NULL)
        MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

    hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(hwndObject, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

    return hwndObject;
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


void changeItemInListView(HWND hwndObject, int rowId, list<string> &cellValue) {
	LVITEM lv = {0};
	lv.iItem = rowId;	

	if (hwndObject == 0)
		cout << "denna hwnd is not defined" << endl;

	ListView_SetItem(hwndObject, &lv);
	int colVal = 0;
	char cellValueCopy[100];

	for (list<string>::iterator itstr = cellValue.begin(); itstr != cellValue.end(); itstr++)
	{
		strcpy(cellValueCopy, itstr->c_str());
		ListView_SetItemText(hwndObject, rowId, colVal++, cellValueCopy);
	}
}

void addItemInListView(HWND hwndObject, int rowId, list<string> &cellValue) {
	LVITEM lv = {0};
	lv.iItem = rowId;


	if (hwndObject == 0)
		cout << "denna hwnd is not defined" << endl;

	ListView_InsertItem(hwndObject, &lv);
	
	int colVal = 0;
	char cellValueCopy[100];

	for (list<string>::iterator itstr = cellValue.begin(); itstr != cellValue.end(); itstr++)
	{
		strcpy(cellValueCopy, itstr->c_str());
		ListView_SetItemText(hwndObject, rowId, colVal++, cellValueCopy);
	}
}

	// https://msdn.microsoft.com/en-us/library/windows/desktop/ff485960(v=vs.85).aspx 
void addItemInListView(int listViewType, list<string> &cellValue)
{

	HWND hwndObject;
	int itemNum = -1;
	switch(listViewType)
	{
		case IDC_VERTICE_LISTVIEW: {
			hwndObject = hwndVertexListView;
			break;
		}
		case IDC_EDGE_LISTVIEW:
			hwndObject = hwndEdgeListView;
			break;
		case IDC_FACE_LISTVIEW:
			hwndObject = hwndFaceListView;
			break;
		default:
			cout << "fel listViewType param" << endl;
			return;
	}


	list<string>::const_iterator cellValueIterator = cellValue.begin();
	string firstString = *cellValueIterator;
	const char firstChar = firstString[0];
	if (listViewType == IDC_VERTICE_LISTVIEW)
	{
		switch(firstChar)
		{
			case 'V':
				changeItemInListView(hwndObject, 0, cellValue);
				break;
			case 'E':
				changeItemInListView(hwndObject, 1, cellValue);
				break;
			case 'F':
				changeItemInListView(hwndObject, 2, cellValue);
				break;
			default: {
				int numOfRows = ListView_GetItemCount(hwndObject);
				itemNum = atoi(firstString.c_str());
				char LVtexten[5];
				int LVtextenAsInt;

				for (int rowNum=3; rowNum<numOfRows; rowNum++)
				{
					ListView_GetItemText(hwndObject, rowNum, 0, LVtexten, 5);
					LVtextenAsInt = atoi(LVtexten);
					if (LVtextenAsInt == itemNum) {
						changeItemInListView(hwndObject, rowNum, cellValue);
						return;
					} else if (LVtextenAsInt > itemNum) {
						addItemInListView(hwndObject, rowNum-1, cellValue);
						return;
					}
				}
				addItemInListView(hwndObject, numOfRows, cellValue);
			}
		}
	} else { 
		int numOfRows = ListView_GetItemCount(hwndObject);
		itemNum = atoi(cellValueIterator->c_str());
		char LVtexten[5];
		int LVtextenAsInt;

		for (int rowNum=0; rowNum<numOfRows; rowNum++)
		{
			ListView_GetItemText(hwndObject, rowNum, 0, LVtexten, 5);
			LVtextenAsInt = atoi(LVtexten);
			if (LVtextenAsInt == itemNum) {
				changeItemInListView(hwndObject, rowNum, cellValue);
				return;
			} else if (LVtextenAsInt > itemNum) {
				addItemInListView(hwndObject, rowNum-1, cellValue);
				return;
			}
		}
		addItemInListView(hwndObject, numOfRows, cellValue);
		//itemNum = atoi(cellValueIterat->c_str());
	}

		// VC, EC, FC

	//cout << "num = " << hwndObject.getAdapter().getCount() << endl;
	//cout << "num of rows = " << ListView_GetItemCount(hwndObject) << endl;



}



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
        hwndVertexListView = createListView(hwnd, "comesing", 0, 250, 0, 150, IDC_VERTICE_LISTVIEW, columnNames);

        columnNames.clear();
        columnNames.push_back(ListViewColumn("id", 30));
		columnNames.push_back(ListViewColumn("fr", 40));
		columnNames.push_back(ListViewColumn("to", 40));
		columnNames.push_back(ListViewColumn("next", 40));
		columnNames.push_back(ListViewColumn("prev", 40));
		columnNames.push_back(ListViewColumn("oppo", 40));
		columnNames.push_back(ListViewColumn("face", 40));
		columnNames.push_back(ListViewColumn("len", 40));
		columnNames.push_back(ListViewColumn("k", 40));
		columnNames.push_back(ListViewColumn("d", 40));
		columnNames.push_back(ListViewColumn("l0", 40));
		columnNames.push_back(ListViewColumn("theta", 40));
        hwndEdgeListView = createListView(hwnd, "comesing", 0, 450, 150, 100, IDC_EDGE_LISTVIEW, columnNames);

		list<string> val;
		val.push_back("VC");
		addItemInListView(IDC_VERTICE_LISTVIEW, val);

		val.clear();
		val.push_back("EC");
		addItemInListView(IDC_VERTIC E_LISTVIEW, val);

		val.clear();
		val.push_back("FC");
		addItemInListView(IDC_VERTICE_LISTVIEW, val);

        /*columnNames.clear();
        columnNames.push_back(ListViewColumn("id", 50));
		columnNames.push_back(ListViewColumn("fr", 30));
		columnNames.push_back(ListViewColumn("len", 30));
		columnNames.push_back(ListViewColumn("type", 30));
		columnNames.push_back(ListViewColumn("flat", 30));
        createMultiEdit2(hwnd, "comesing", 200, 200, 0, 100, IDC_FACE_LISTVIEW, columnNames);*/


        createEdit(hwnd, "tjej", 250, 100, 50, 24, IDC_TEXT);
        createButton(hwnd, "#Mess in Cue", 350, 100, 50, 24, IDC_MAIN_BUTTON);
        createButton(hwnd, "Testa 1", 250, 100, 75	, 24, IDC_TEST1_BUTTON);
        //createButton(hwnd, "#Mess in Cue", 350, 100, 50, 24, IDC_MAIN_BUTTON);
        
        //createCheckbox(hwnd, "vertex-centered", 0, 200, 150, 30, IDC_CHECK_BOX_1);
        //createCheckbox(hwnd, "edge-centered", 0, 200, 180, 30, IDC_CHECK_BOX_2);
        //createCheckbox(hwnd, "face-centered", 0, 200, 210, 30, IDC_CHECK_BOX_3);
        //createListbox(hwnd, "gay", 0, 200, 240, 100, IDC_LISTBOX);
        //createButton(hwnd, "taka awayness", 0, 100, 350, 24, IDC_DEL_BUTTON);
        
        createButton(hwnd, "<< Prev", 250, 100, 100, 24, IDC_PREV_BUTTON);
        createButton(hwnd, "Next >>", 350, 100, 100, 24, IDC_NEXT_BUTTON);
	} else if (newMode == 1) {
		ShowWindow(GetDlgItem(hwnd, IDC_MAIN_BUTTON), SW_HIDE);
	} else if (newMode == 2) {
		ShowWindow(GetDlgItem(hwnd, IDC_MAIN_BUTTON), SW_SHOW);
	}
}

void try1()
{
	char texten[100];
	cout << "try1"  << endl;
	int itemCount = ListView_GetItemCount(hwndVertexListView);
	cout << "item Count: " << itemCount << endl;

	//VOID ListView_SetItemText(HWND hwnd,int i,int iSubItem,LPCTSTR pszText);
	ListView_GetItemText(hwndVertexListView, 1, 1, texten, 100);
	cout << "texten: " << texten << endl;

}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ff485960(v=vs.85).aspx 
