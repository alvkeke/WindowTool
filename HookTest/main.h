#pragma once
#include <fstream>
#include <string>
#include <vector>


#define FILENAME "MarkConfigure.conf"
#define WNDCLASS "ALV_KEYTOOL_WND"
#define WNDTITLE "按键快捷操作工具"

using namespace std;



LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

