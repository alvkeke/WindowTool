#include "hooks.h"


HWND fWin;
RECT winRect;
POINT pDown, pNow;
bool bIsFuncKeyDown;
bool bWant2Move;
bool bWant2Size;

HHOOK kHook, mHook;

vector<string> MarkedClasses;

bool isListening;


int initHook(HINSTANCE hInstance, vector<string> s)
{

	MarkedClasses = s;

	bIsFuncKeyDown = false;
	bWant2Move = false;
	bWant2Size = false;


	// 设置键盘全局监听
	kHook = SetWindowsHookEx(
		WH_KEYBOARD_LL, // 监听类型【键盘】
		KeyHookProc,   // 处理函数
		hInstance,      // 当前实例句柄
		0               // 监听窗口句柄(NULL为全局监听)
	);

	mHook = SetWindowsHookEx(
		WH_MOUSE_LL,
		MouseHookProc,
		hInstance,
		0
	);


	if (kHook == NULL)
	{
		return 1;
	}

	if (mHook == NULL)
	{
		return 1;
	}

	return 0;
}

void setListenState(bool foo)
{
	isListening = foo;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

	if (isListening && nCode >= 0)
	{
		/*
		if (p->vkCode == HOOK_KEY_FUNC)
		{
			if (!bIsFuncKeyDown)
			{

				if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN)
				{
					bIsFuncKeyDown = true;
					bWant2Move = false;
					bWant2Size = false;
				}

			}
			if (bIsFuncKeyDown && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{

				bIsFuncKeyDown = false;

				RECT rNow;
				GetWindowRect(fWin, &rNow);

				if (rNow.bottom != winRect.bottom || rNow.left != winRect.left || rNow.right != winRect.right || rNow.top != winRect.top)
				{
					// 窗口状态发生改变，则：
					winRect = rNow;
				}
			}

			// 所有func键事件都屏蔽
			return 1;
		}
		else if (bIsFuncKeyDown && p->vkCode == HOOK_KEY_SWITCH)
		{
			if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) 
			{
				HWND hwnd = FindWindow("ALV_KEYTOOL_WND", "按键快捷操作工具");
				SendMessage(hwnd, WM_CALLBACK_DISABLE, 0, 0);
			}
			return 1;
		}
		*/

		if (bIsFuncKeyDown)
		{
			if (p->vkCode == HOOK_KEY_FUNC && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{
				bIsFuncKeyDown = false;

				RECT rNow;
				GetWindowRect(fWin, &rNow);

				if (rNow.bottom != winRect.bottom || rNow.left != winRect.left || rNow.right != winRect.right || rNow.top != winRect.top)
				{
					// 窗口状态发生改变，则：
					winRect = rNow;
				}
			}
			else if (p->vkCode == HOOK_KEY_SWITCH)
			{
				if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
				{
					HWND hwnd = FindWindow("ALV_KEYTOOL_WND", "按键快捷操作工具");
					SendMessage(hwnd, WM_CALLBACK_DISABLE, 0, 0);
				}
			}
			
			return 1;
		}
		else
		{
			if (p->vkCode == HOOK_KEY_FUNC && (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN))
			{
				bIsFuncKeyDown = true;
				bWant2Move = false;
				bWant2Size = false;
				return 1;
			}
		}
	}

	return CallNextHookEx(kHook, nCode, wParam, lParam);
}


LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSLLHOOKSTRUCT p = (LPMSLLHOOKSTRUCT)lParam;
	POINT   pt = p->pt;

	if (isListening && nCode >= 0 && bIsFuncKeyDown)
	{
		int newX, newY;
		int newW, newH;
		GetCursorPos(&pNow);
		newX = winRect.left + pNow.x - pDown.x;
		newY = winRect.top + pNow.y - pDown.y;

		newW = winRect.right - winRect.left + pNow.x - pDown.x;
		newH = winRect.bottom - winRect.top + pNow.y - pDown.y;

		if (bWant2Move && wParam == WM_MOUSEMOVE)
		{
			SetWindowPos(fWin, 0, newX, newY, 0, 0, SWP_NOSIZE);
		}

		if (bWant2Size && wParam == WM_MOUSEMOVE)
		{
			SetWindowPos(fWin, 0, 0, 0, newW, newH, SWP_NOMOVE);
		}

		if (wParam == WM_LBUTTONDOWN)
		{
			GetCursorPos(&pDown);
			fWin = WindowFromPoint(pDown);
			if (fWin == 0) goto exitl;
			while (GetParent(fWin)) {
				fWin = GetParent(fWin);
			}

			SetFocus(fWin);

			char pname[256];
			RealGetWindowClass(fWin, pname, 255);
			for (int i = 0; i < MarkedClasses.size(); i++) {
				if (MarkedClasses.at(i)._Equal(pname)) goto exitl;
			}

			if (IsZoomed(fWin)) {
				ShowWindow(fWin, SW_SHOWNORMAL);
			}

			GetWindowRect(fWin, &winRect);
			bWant2Move = true;
			if (!strcmp(pname, "Windows.UI.Core.CoreWindow")) bWant2Move = false;
			//for (int i = 0; i<)
			return 1;
		}
		if (wParam == WM_RBUTTONDOWN)
		{
			GetCursorPos(&pDown);
			fWin = WindowFromPoint(pDown);
			if (fWin == 0) goto exitl;
			while (GetParent(fWin)) {
				fWin = GetParent(fWin);
			}

			SetFocus(fWin);

			char pname[256];
			RealGetWindowClass(fWin, pname, 255);
			for (int i = 0; i < MarkedClasses.size(); i++) {
				if (MarkedClasses.at(i)._Equal(pname)) goto exitl;
			}

			if (IsZoomed(fWin)) {
				ShowWindow(fWin, SW_SHOWNORMAL);
			}

			GetWindowRect(fWin, &winRect);
			bWant2Size = true;
			if (!strcmp(pname, "Windows.UI.Core.CoreWindow")) bWant2Size = false;
			return 1;
		}
		if (bWant2Move && wParam == WM_LBUTTONUP)
		{
			SetWindowPos(fWin, 0, newX, newY, 0, 0, SWP_NOSIZE);
			bWant2Move = false;
			return 1;
		}
		if (bWant2Size && wParam == WM_RBUTTONUP)
		{
			SetWindowPos(fWin, 0, 0, 0, newW, newH, SWP_NOMOVE);
			bWant2Size = false;
			return 1;
		}

	}

exitl:
	return CallNextHookEx(mHook, nCode, wParam, lParam);
}
