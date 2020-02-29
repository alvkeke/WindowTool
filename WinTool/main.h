#pragma once
#include <fstream>
#include <string>
#include <vector>


#define FILENAME			"MarkConfigure.conf"
#define WNDCLASS			"ALV_KEYTOOL_WND"
#define WNDTITLE			"按键快捷操作工具"

#define TITLE_CB_LISTEN		"启用快捷操作"

#define HELP_TEXT	"帮助：\n本程序通过“win键”与鼠标组合实现各种功能，所有功能都在“win键”被按下的情况下进行。现有功能如下：\n1.鼠标左键拖动：移动窗口\n2.鼠标右键拖动：缩放窗口\n3.鼠标中键按下：最大/小化窗口\n4.鼠标滚轮向上：置顶窗口\n5.鼠标滚轮向下：取消窗口置顶"

/* 这里是控件ID，被设置在菜单项中 */
#define ID_CB_LISTEN		10001
#define ID_CB_TIPS			10002

#define ID_LB_KEYDOWN		10004

using namespace std;

void showPopupMenu(HWND hwnd, HMENU hmPop);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

