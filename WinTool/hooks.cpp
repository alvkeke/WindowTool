#include "hooks.h"


// �����������ھ��
HWND hwnd;

// Hook
HHOOK kHook, mHook;

// �����������Ϣ
HWND fWin;
RECT winRect;
POINT pDown, pNow;

// ��ǰ״̬
bool bIsListening;
bool bIsFuncKeyDown;
bool bWant2Move;
bool bWant2Size;

bool bIsFuncKeySet;
bool bBlockFuncKey;

// ���ӹ��ܿ���
// bool bIsEnableF4;
// bool bIsEnableTab;

// ���εĴ���
vector<string> MarkedClasses;


int initHook(HINSTANCE hInstance, HWND hWnd, vector<string> s)
{

	MarkedClasses = s;

	bIsFuncKeyDown = false;
	bWant2Move = false;
	bWant2Size = false;
	bIsFuncKeySet = false;
	bBlockFuncKey = false;


	// ���ü���ȫ�ּ���
	kHook = SetWindowsHookEx(
		WH_KEYBOARD_LL, // �������͡����̡�
		KeyHookProc,   // ������
		hInstance,      // ��ǰʵ�����
		0               // �������ھ��(NULLΪȫ�ּ���)
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

	hwnd = hWnd;

	return 0;
}

void setListenState(bool foo)
{
	bIsListening = foo;
}

/*
void setEnableF4(bool foo)
{
	bIsEnableF4 = foo;
}

void setEnableTab(bool foo)
{
	bIsEnableTab = foo;
}
*/

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT kbdata = (PKBDLLHOOKSTRUCT)lParam;

	if (nCode >= 0)
	{
		// ����win������ʱ������, ������������
		if (!bIsFuncKeyDown && kbdata->vkCode == HOOK_KEY_FUNC && (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN))
		{
			bIsFuncKeyDown = true;
			bIsFuncKeySet = false;
			bWant2Move = false;
			bWant2Size = false;
			bBlockFuncKey = false;
			return 1;
		}
		else if (bIsFuncKeyDown)
		{
			if (kbdata->vkCode == HOOK_KEY_FUNC) 
			{
				if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
				{
					
					if (!bIsFuncKeySet && !bBlockFuncKey)
					{
						keybd_event(HOOK_KEY_FUNC, 0, 0, 0);
						keybd_event(HOOK_KEY_FUNC, 0, KEYEVENTF_KEYUP, 0);
					}
					else if (bIsFuncKeySet)
					{
						keybd_event(HOOK_KEY_FUNC, 0, KEYEVENTF_KEYUP, 0);
						bIsFuncKeySet = false;
					}

					bIsFuncKeyDown = false;
				}   
			}
			else
			{
				if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
				{
					// ֻ���ڵ�һ�ΰ���ʱ����win��������
					
					bIsFuncKeySet = true;
					keybd_event(HOOK_KEY_FUNC, 0, 0, 0);
					keybd_event(kbdata->vkCode, 0, 0, 0);
				}
				else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
				{
					keybd_event(kbdata->vkCode, 0, KEYEVENTF_KEYUP, 0);
				}
			}

			return 1;
		}
	}

	return CallNextHookEx(kHook, nCode, wParam, lParam);
}


LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSLLHOOKSTRUCT p = (LPMSLLHOOKSTRUCT)lParam;
	POINT   pt = p->pt;

	if (bIsListening && nCode >= 0 && bIsFuncKeyDown)
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
			bBlockFuncKey = true;

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
			bBlockFuncKey = true;

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
