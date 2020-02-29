
#include "hooks.h"
#include "trayicon.h"
#include "resource.h"
#include "main.h"


bool bEnableListen;
bool bEnableTips;

ATOM hotkeyId;
HMENU hmPop;


HINSTANCE hInstance;
HWND hcbListen;
HWND hcbTips;

HWND hlbKeyDown;

int WINAPI CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	::hInstance = hInstance;

	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
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

	if (bEnableTips)
		CheckMenuItem(hmPop, IDM_ENABLE_TIP, MF_CHECKED);
	else
		CheckMenuItem(hmPop, IDM_ENABLE_TIP, MF_UNCHECKED);

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
		bEnableTips = false;

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
			20, 20, 200, 30, hwnd, (HMENU)ID_CB_LISTEN, hInstance, NULL);

		hcbTips = CreateWindowEx(0, "BUTTON", TITLE_CB_LISTEN, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			20, 60, 200, 30, hwnd, (HMENU)ID_CB_TIPS, hInstance, NULL);

		hlbKeyDown = CreateWindow("LISTBOX", "LISTBOX", WS_CHILD | WS_VISIBLE, 
			20, 110, 200, 120, hwnd, (HMENU)ID_LB_KEYDOWN, hInstance, NULL);

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

		if (bEnableTips)
		{
			SendMessage(hcbTips, BM_SETCHECK, BST_CHECKED, 0);
		}
		else
		{
			SendMessage(hcbTips, BM_SETCHECK, BST_UNCHECKED, 0);
		}

		break;

	case WM_SIZE:
		int cyClient, cxClient;
		cyClient = HIWORD(lParam); // 获得客户区高度
		cxClient = LOWORD(lParam);
		SetWindowPos(hlbKeyDown, 0, 0, 0, cxClient - 40, cyClient - 130, SWP_NOMOVE);
		SetWindowPos(hcbListen, 0, 0, 0, cxClient - 40, 30, SWP_NOMOVE);
		SetWindowPos(hcbTips, 0, 0, 0, cxClient - 40, 30, SWP_NOMOVE);
		break;

	case WM_RBUTTONUP:
		showPopupMenu(hwnd, hmPop);
		break;

	case WM_KEYUP:
		// 窗口显示的时候，按下退出键可以隐藏窗口
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(hwnd, SW_HIDE);
		}
		break;

	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return 1;
		
	case WM_CALLBACK_SWITCH:
		bEnableListen = !bEnableListen;
		setListenState(bEnableListen);
		if (bEnableListen)
		{
			ChangeTrayIcon(IDI_ICON_ENB);
			if (bEnableTips) 
			{
				ShowBalloonTip("已启用快捷操作方式", "WinTool", 100, 0);
			}
			SendMessage(hcbListen, BM_SETCHECK, BST_CHECKED, 0);
		}
		else
		{
			ChangeTrayIcon(IDI_ICON_BLK);
			SendMessage(hcbListen, BM_SETCHECK, BST_UNCHECKED, 0);
		}

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

		case ID_CB_TIPS:
			result = SendMessage(hcbTips, BM_GETCHECK, 0, 0);
			bEnableTips = (result == BST_CHECKED);
			setListenState(bEnableTips);

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

		case IDM_ENABLE_TIP:
			bEnableTips = !bEnableTips;
			if (bEnableTips)
			{
				SendMessage(hcbTips, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				SendMessage(hcbTips, BM_SETCHECK, BST_UNCHECKED, 0);
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