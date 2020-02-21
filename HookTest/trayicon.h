#include <Windows.h>

#define WM_TRAY_ICON WM_USER + 1

void initTrayIcon(HWND hwnd);

void delTrayIcon();

BOOL ChangeTrayIcon(int id);

BOOL ShowBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle, UINT uTimeout, DWORD dwInfoFlags = 1);