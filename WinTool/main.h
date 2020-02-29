#pragma once
#include <fstream>
#include <string>
#include <vector>


#define FILENAME			"MarkConfigure.conf"
#define WNDCLASS			"ALV_KEYTOOL_WND"
#define WNDTITLE			"按键快捷操作工具"

#define TITLE_CB_LISTEN		"启用HOOK监听"
#define TITLE_CB_F4			"启用 Alt+F4"
#define TITLE_CB_TAB		"启用 Alt+Tab"


/* 这里是控件ID，被设置在菜单项中 */
#define ID_CB_LISTEN		10001
#define ID_CB_F4			10002
#define ID_CB_TAB			10003

#define ID_LB_KEYDOWN		10004

using namespace std;

void showPopupMenu(HWND hwnd, HMENU hmPop);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

