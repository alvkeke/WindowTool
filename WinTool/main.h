#pragma once
#include <fstream>
#include <string>
#include <vector>


#define FILENAME			"MarkConfigure.conf"
#define WNDCLASS			"ALV_KEYTOOL_WND"
#define WNDTITLE			"按键快捷操作工具"

#define TITLE_CB_LISTEN		"启用快捷操作"


/* 这里是控件ID，被设置在菜单项中 */
#define ID_CB_LISTEN		10001
#define ID_CB_TIPS			10002

#define ID_LB_KEYDOWN		10004

using namespace std;

void showPopupMenu(HWND hwnd, HMENU hmPop);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

