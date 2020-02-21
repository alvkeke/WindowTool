#include "trayicon.h"
#include "resource.h"

static NOTIFYICONDATA icon;

void initTrayIcon(HWND hwnd)
{
	icon.cbSize = sizeof(icon);
	icon.hWnd = hwnd;
	icon.uID = 0;
	icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	icon.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	icon.uCallbackMessage = WM_TRAY_ICON;

	Shell_NotifyIcon(NIM_ADD, &icon);
}

void delTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &icon);
}
