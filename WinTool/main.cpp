
#include "hooks.h"
#include "trayicon.h"
#include "resource.h"
#include "main.h"


bool bEnableListen;

ATOM hotkeyId;
HMENU hmPop;


HINSTANCE hInstance;
HWND hcbListen;
HWND hcbF4;
HWND hcbTab;

HWND hlbKeyDown;

int WINAPI CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	::hInstance = hInstance;

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

	HMENU hmRoot = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENUROOT));
	hmPop = GetSubMenu(hmRoot, 0);

	//ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);                //更新窗口


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

	if (initHook(hInstance, hwnd, MarkedClasses))
	{
		MessageBox(0, "监听出错，程序已退出。\nError in listen event, application exit.", TEXT("错误"), MB_OK | MB_ICONERROR);
		return 1;
	}

	// hotkeyId = GlobalAddAtom("switchStateKey") - 0xc000;
	// RegisterHotKey(hwnd, hotkeyId, HOT_KEY_FUNC, HOOK_KEY_SWITCH);

	// 5. 消息循环
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

void showPopupMenu(HWND hwnd, HMENU hmPop)
{
	POINT p;
	GetCursorPos(&p);
	SetForegroundWindow(hwnd);
	if (bEnableListen)
		CheckMenuItem(hmPop, IDM_ENABLE_LISTEN, MF_CHECKED);
	else
		CheckMenuItem(hmPop, IDM_ENABLE_LISTEN, MF_UNCHECKED);

	TrackPopupMenu(hmPop, TPM_RIGHTBUTTON, p.x, p.y, NULL, hwnd, NULL);

}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int result;
	char str[20] = "helloasdjfwef";

	switch (uMsg)    //消息选择
	{

	case WM_CREATE:
		// hInstance = GetModuleHandle(NULL);
			
		// 添加托盘图标
		initTrayIcon(hwnd);

		// 默认状态，以后可以修改为从配置文件中读取
		bEnableListen = true;

		setListenState(bEnableListen);


		/* 窗口居中，并置顶显示 */
		int px, py, width, height;
		width = 400;
		height = 400;
		px = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		py = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
		SetWindowPos(hwnd, (HWND)-1, px, py, width, height, 0);
		// ShowWindow(hwnd, SW_SHOW);

		/* 创建控件 */
		hcbListen = CreateWindowEx(0, "BUTTON", TITLE_CB_LISTEN, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			10, 10, 200, 30, hwnd, (HMENU)ID_CB_LISTEN, hInstance, NULL);

		hcbTab = CreateWindowEx(0, "BUTTON", TITLE_CB_TAB, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			30, 50, 200, 30, hwnd, (HMENU)ID_CB_TAB, hInstance, NULL);

		hcbF4 = CreateWindowEx(0, "BUTTON", TITLE_CB_F4, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			30, 90, 200, 30, hwnd, (HMENU)ID_CB_F4, hInstance, NULL);

		hlbKeyDown = CreateWindow("LISTBOX", "LISTBOX", WS_CHILD | WS_VISIBLE, 
			30, 130, 200, 120, hwnd, (HMENU)ID_LB_KEYDOWN, hInstance, NULL);

		if (bEnableListen)
		{
			ChangeTrayIcon(IDI_ICON_ENB);
			SendMessage(hcbListen, BM_SETCHECK, BST_CHECKED, 0);
		}
		else
		{
			ChangeTrayIcon(IDI_ICON_BLK);
			SendMessage(hcbListen, BM_SETCHECK, BST_UNCHECKED, 0);
		}

		break;

	case WM_SIZE:
		int cyClient;
		cyClient = HIWORD(lParam); // 获得客户区高度
		SetWindowPos(hlbKeyDown, 0, 0, 0, 200, cyClient - 140, SWP_NOMOVE);
		break;

	case WM_RBUTTONUP:
		showPopupMenu(hwnd, hmPop);
		break;

	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return 1;

	case WM_HOTKEY:
		// 不屏蔽按键时，按下则为启动
		if (wParam == hotkeyId)
		{
			bEnableListen = true;
			keybd_event(HOOK_KEY_FUNC, 0, KEYEVENTF_KEYUP, 0);
			setListenState(true);
			ShowBalloonTip("快捷操作已  启用（switch on）。", "状态更改", 1500);
			ChangeTrayIcon(IDI_ICON_ENB);
		}
		break;
		
	case WM_CALLBACK_DISABLE:
		// 这里是程序启用时，按下则为关闭
		bEnableListen = false;
		// setListenState(false);	//这个方法能够在hooks.cpp中被调用，故不需要在此处调用
		ChangeTrayIcon(IDI_ICON_BLK);
		
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case ID_CB_LISTEN:
			result = SendMessage(hcbListen, BM_GETCHECK, 0, 0);
			bEnableListen = (result == BST_CHECKED);
			setListenState(bEnableListen);
			if (bEnableListen)
			{
				ChangeTrayIcon(IDI_ICON_ENB);
			}
			else
			{
				ChangeTrayIcon(IDI_ICON_BLK);
			}
			break;

		case ID_LB_KEYDOWN:
			
			break;

		case IDM_ENABLE_LISTEN:
			bEnableListen = !bEnableListen;
			setListenState(bEnableListen);
			if (bEnableListen)
			{
				ChangeTrayIcon(IDI_ICON_ENB);
				SendMessage(hcbListen, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				ChangeTrayIcon(IDI_ICON_BLK);
				SendMessage(hcbListen, BM_SETCHECK, BST_UNCHECKED, 0);
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
			showPopupMenu(hwnd, hmPop);
		}

		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	//对于我们不想处理的消息, 比如鼠标在窗口上移动时发出的消息
	//我们就原样传给默认的窗口消息处理函数处理, 不然应用程序会失去响应
}