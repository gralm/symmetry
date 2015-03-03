#ifndef GUI_H__
#define GUI_H__

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <list>
#include <cstring>
#include <commctrl.h>		// WC_LISTVIEW is defined here




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

#define IDC_TEST1_BUTTON		301
#define IDC_TEST2_BUTTON		302
#define IDC_TEST3_BUTTON		303

#define IDC_VERTICE_LISTVIEW	201
#define IDC_EDGE_LISTVIEW		202
#define IDC_FACE_LISTVIEW		203




void createEdit(HWND hwnd, const char* text, int left, int width, int top, int height, int id);

	  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms633574(v=vs.85).aspx#system
void createButton(HWND hwnd, const char* text, int left, int width, int top, int height, int id);

void createMultiEdit(HWND hwnd, const char* text, int left, int width, int top, int height, int id);
void createListbox(HWND hwnd, const char* text, int left, int width, int top, int height, int id);

void createCheckbox(HWND hwnd, const char* text, int left, int width, int top, int height, int id);
//void createMultiEdit2(HWND hwnd, const char* text, int left, int width, int top, int height, int id, std::list<char*> columnName);
void changeGuiMode(HWND hwnd, int presentMode, int newMode);


void setItemInListView(int listViewType, std::list<std::string> &cellValue);
void addItemInListView(HWND hwndObject, int rowId, std::list<std::string> &cellValue);
void try1();

std::list<std::string> stringSplit(const char *str, int len, char splitChar);
std::list<std::string> stringSplit(std::string str, char splitChar);


#endif
