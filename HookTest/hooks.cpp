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

// ���ӹ��ܿ���
bool bIsEnableF4;
bool bIsEnableTab;

// ���εĴ���
vector<string> MarkedClasses;


int initHook(HINSTANCE hInstance, HWND hWnd, vector<string> s)
{

	MarkedClasses = s;

	bIsFuncKeyDown = false;
	bWant2Move = false;
	bWant2Size = false;
	bIsFuncKeySet = false;


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

void setEnableF4(bool foo)
{
	bIsEnableF4 = foo;
}

void setEnableTab(bool foo)
{
	bIsEnableTab = foo;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT kbdata = (PKBDLLHOOKSTRUCT)lParam;

	if (bIsListening && nCode >= 0)
	{

		if (bIsFuncKeyDown)
		{
			// �����ܣ�Func���ɿ�
			if (kbdata->vkCode == HOOK_KEY_FUNC && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{
				bIsFuncKeyDown = false;

				RECT rNow;
				GetWindowRect(fWin, &rNow);

				if (rNow.bottom != winRect.bottom || rNow.left != winRect.left || rNow.right != winRect.right || rNow.top != winRect.top)
				{
					// ����״̬�����ı䣬��
					winRect = rNow;
				}
				// ����ڴ�����������Fun�����£������ɿ�ʱ��Ҫ�ڴ����������ɿ�
				if (bIsFuncKeySet)
				{
					keybd_event(HOOK_KEY_FUNC, 0, KEYEVENTF_KEYUP, 0);
					bIsFuncKeySet = false;
				}
			}
			// �л��������ص�hook
			else if (kbdata->vkCode == HOOK_KEY_SWITCH)
			{
				if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
				{
					SendMessage(hwnd, WM_CALLBACK_DISABLE, 0, 0);
					setListenState(false);
				}
			}
			// ģ��alt+f4
			else if (bIsEnableF4 && kbdata->vkCode == VK_F4 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
			{

				if (bIsFuncKeySet)
				{
					keybd_event(VK_F4, 0, 0, 0);
				}
				else
				{
					keybd_event(HOOK_KEY_FUNC, 0, 0, 0);
					keybd_event(VK_F4, 0, 0, 0);
					bIsFuncKeySet = true;
				}
			}
			else if (bIsEnableF4 && kbdata->vkCode == VK_F4 && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{
				if (bIsFuncKeySet)
				{
					keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
				}
			}
			// ģ��alt+tab���л�Ӧ��
			else if (bIsEnableTab && kbdata->vkCode == VK_TAB && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
			{
				if (bIsFuncKeySet)
				{
					keybd_event(VK_TAB, 0, 0, 0);
				}
				else
				{
					keybd_event(HOOK_KEY_FUNC, 0, 0, 0);
					keybd_event(VK_TAB, 0, 0, 0);
					bIsFuncKeySet = true;
				}
			}
			else if (bIsEnableTab && kbdata->vkCode == VK_TAB && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{
				if (bIsFuncKeySet)
				{
					keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
				}
			}
			/* ��ʱģ�ⷽ����Ĺ�����ʱ����Ч����Ҫ�޸Ĵ���*/
			//	���������
			else if (kbdata->vkCode == HOOK_KEY_UP && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
			{
				keybd_event(VK_UP, MapVirtualKey(VK_UP, 0), 0, 0);
			}
			else if (kbdata->vkCode == HOOK_KEY_DOWN && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
			{
				keybd_event(VK_DOWN, 0, 0, 0);
			}
			else if (kbdata->vkCode == HOOK_KEY_LEFT && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
			{
				keybd_event(VK_LEFT, 0, 0, 0);
			}
			else if (kbdata->vkCode == HOOK_KEY_RIGHT && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
			{
				keybd_event(VK_RIGHT, 0, 0, 0);
			}
			// ������ɿ�
			else if (kbdata->vkCode == HOOK_KEY_UP && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{
				keybd_event(VK_UP, MapVirtualKey(VK_UP, 0), KEYEVENTF_KEYUP, 0);
			}
			else if (kbdata->vkCode == HOOK_KEY_DOWN && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{
				keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
			}
			else if (kbdata->vkCode == HOOK_KEY_LEFT && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{
				keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
			}
			else if (kbdata->vkCode == HOOK_KEY_RIGHT && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
			{
				keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
			}
			// ������
			else
			{
				// HWND hwnd = FindWindow("ALV_KEYTOOL_WND", "������ݲ�������");
				if (IsWindowVisible(hwnd)) 
				{
					if (kbdata->vkCode == HOOK_KEY_FUNC) return 1;
					// for test
					char nums[20];
					sprintf_s(nums, 20, "%d:%x,%x", kbdata->vkCode, kbdata->vkCode, (int)wParam);
					MessageBox(0, nums, "", 0);
				}

			}
			
			return 1;
		}
		else
		{
			// �����ܣ�Func������
			if (kbdata->vkCode == HOOK_KEY_FUNC && (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN))
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
