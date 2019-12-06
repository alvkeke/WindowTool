#include <fstream>
#include <string>
#include <vector>
#include "hooks.h"
#include "trayicon.h"

#define FILENAME "MarkConfigure.conf"

using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)    //消息选择
	{
		
	case WM_USER:
		if (lParam == WM_LBUTTONUP) {
			MessageBox(hwnd, TEXT("Win32 API 实现系统托盘程序,双击托盘可以退出!"), "", MB_OK);
		}

		if (lParam == WM_RBUTTONDBLCLK) {
			int res = MessageBox(hwnd, "是否退出程序？", "退出", MB_YESNO);
			if (res == IDYES) {
				delTrayIcon();
				PostQuitMessage(0);
			}
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	//对于我们不想处理的消息, 比如鼠标在窗口上移动时发出的消息
	//我们就原样传给默认的窗口消息处理函数处理, 不然应用程序会失去响应
}


int WINAPI CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{


	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = NULL;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "HIDE_MY_WIND";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Register Class Failed!", NULL, MB_OK | MB_ICONERROR);
		return 1;
	}

	HWND hwnd = CreateWindowEx(0, "HIDE_MY_WIND", "mouse tool", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, hInstance, NULL);

	if (!hwnd) {
		MessageBox(NULL, "CreateWindowEx Failed!", NULL, MB_OK);
		return 2;
	}

	//ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);                //更新窗口


	// 添加托盘图标
	initTrayIcon(hwnd);

	// 加载需要屏蔽窗口的顶层窗口类名
	vector<string> MarkedClasses;
	
	ifstream conffile(FILENAME);
	if (!conffile) 
	{
		ofstream file(FILENAME);
		if (file.is_open()) {
			file << "Shell_TrayWnd" << endl;
			file << "Program" << endl;
			file.close();
		}
	}
	else 
	{
		string stmp;
		while (conffile >> stmp) {
			MarkedClasses.push_back(stmp);
		}
	}


	if (initHook(hInstance, MarkedClasses))
	{
		MessageBox(0, "监听出错，程序已退出。\nError in listen event, application exit.", TEXT("错误"), MB_OK | MB_ICONERROR);
		return 1;
	}
	

	// 5. 消息循环
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}