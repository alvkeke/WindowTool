#include <fstream>
#include <string>
#include <vector>
#include "hooks.h"
#include "trayicon.h"
#include "resource.h"

#define FILENAME "MarkConfigure.conf"

using namespace std;

bool isEnable;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance;
	HMENU hmRoot;
	static HMENU hmPop;

	switch (uMsg)    //��Ϣѡ��
	{

	case WM_CREATE:

		hInstance = GetModuleHandle(NULL);
		hmRoot = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU1));
		hmPop = GetSubMenu(hmRoot, 0);

		break;

	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return 1;
		
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_ENABLE:
			isEnable = !isEnable;
			setListenState(isEnable);
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
	//�������ǲ��봦�����Ϣ, ��������ڴ������ƶ�ʱ��������Ϣ
	//���Ǿ�ԭ������Ĭ�ϵĴ�����Ϣ����������, ��ȻӦ�ó����ʧȥ��Ӧ
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
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
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
	UpdateWindow(hwnd);                //���´���


	// �������ͼ��
	initTrayIcon(hwnd);

	// ������Ҫ���δ��ڵĶ��㴰������
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
		MessageBox(0, "���������������˳���\nError in listen event, application exit.", TEXT("����"), MB_OK | MB_ICONERROR);
		return 1;
	}
	
	setListenState(isEnable);

	// 5. ��Ϣѭ��
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}