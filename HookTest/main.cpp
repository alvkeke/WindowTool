
#include "hooks.h"
#include "trayicon.h"
#include "resource.h"
#include "main.h"


bool isEnable;
ATOM hotkeyId;


int WINAPI CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
	wc.hIconSm = NULL;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = WNDCLASS;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Register Class Failed!", NULL, MB_OK | MB_ICONERROR);
		return 1;
	}

	HWND hwnd = CreateWindowEx(0, WNDCLASS, WNDTITLE, WS_OVERLAPPEDWINDOW,
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
	
	// 默认状态，以后可以修改为从配置文件中读取
	isEnable = true;
	setListenState(isEnable);

	hotkeyId = GlobalAddAtom("switchStateKey") - 0xc000;
	RegisterHotKey(hwnd, hotkeyId, HOT_KEY_FUNC, HOOK_KEY_SWITCH);

	// 5. 消息循环
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance;
	HMENU hmRoot;
	static HMENU hmPop;

	switch (uMsg)    //消息选择
	{

	case WM_CREATE:

		hInstance = GetModuleHandle(NULL);
		hmRoot = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENUROOT));
		hmPop = GetSubMenu(hmRoot, 0);

		break;

	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return 1;

	case WM_HOTKEY:
		// 不屏蔽按键时，按下则为启动
		if (wParam == hotkeyId)
		{
			isEnable = true;
			keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
			setListenState(true);
			ShowBalloonTip("快捷操作已  启用（switch on）。", "状态更改", 1500);
			ChangeTrayIcon(IDI_ICON_ENB);
		}
		break;
		
	case WM_CALLBACK_DISABLE:
		// 这里是程序启用时，按下则为关闭
		isEnable = false;
		setListenState(false);
		ChangeTrayIcon(IDI_ICON_BLK);
		
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_ENABLE:
			isEnable = !isEnable;
			setListenState(isEnable);
			if (isEnable)
			{
				ChangeTrayIcon(IDI_ICON_ENB);
			}
			else
			{
				ChangeTrayIcon(IDI_ICON_BLK);
			}
			break;
		case IDM_EXIT:
			delTrayIcon();
			PostQuitMessage(0);
			break;
		}


		break;
	case WM_TRAY_ICON:

		if (lParam == WM_LBUTTONDBLCLK)
		{
			ShowWindow(hwnd, SW_SHOW);
		}
		else if (lParam == WM_RBUTTONUP)
		{

			POINT p;
			GetCursorPos(&p);
			SetForegroundWindow(hwnd);
			if (isEnable)
			{
				CheckMenuItem(hmPop, IDM_ENABLE, MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hmPop, IDM_ENABLE, MF_UNCHECKED);
			}
			TrackPopupMenu(hmPop, TPM_RIGHTBUTTON, p.x, p.y, NULL, hwnd, NULL);

		}

		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	//对于我们不想处理的消息, 比如鼠标在窗口上移动时发出的消息
	//我们就原样传给默认的窗口消息处理函数处理, 不然应用程序会失去响应
}