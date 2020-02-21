#include "trayicon.h"
#include "resource.h"

static NOTIFYICONDATA icon;

void initTrayIcon(HWND hwnd)
{
	icon.cbSize = sizeof(NOTIFYICONDATA);
	icon.hWnd = hwnd;
	icon.uID = 0;
	icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	icon.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_ENB));
	icon.uCallbackMessage = WM_TRAY_ICON;

	Shell_NotifyIcon(NIM_ADD, &icon);
}

void delTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &icon);
}

BOOL ChangeTrayIcon(int id)
{
	icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	icon.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(id));
	return Shell_NotifyIcon(NIM_MODIFY, &icon);
}

BOOL ShowBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle, UINT uTimeout, DWORD dwInfoFlags)
{
	icon.cbSize = sizeof(NOTIFYICONDATA);
	icon.uFlags = NIF_INFO;
	icon.uTimeout = uTimeout;
	icon.dwInfoFlags = dwInfoFlags;
	lstrcpy(icon.szInfo, szMsg);
	lstrcpy(icon.szInfoTitle, szTitle);

	return Shell_NotifyIcon(NIM_MODIFY, &icon);
}
