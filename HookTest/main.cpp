#include <fstream>
#include <string>
#include <vector>
#include "hooks.h"
#include "trayicon.h"

#define FILENAME "MarkConfigure.conf"

using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)    //��Ϣѡ��
	{
		
	case WM_USER:
		if (lParam == WM_LBUTTONUP) {
			MessageBox(hwnd, TEXT("Win32 API ʵ��ϵͳ���̳���,˫�����̿����˳�!"), "", MB_OK);
		}

		if (lParam == WM_RBUTTONDBLCLK) {
			int res = MessageBox(hwnd, "�Ƿ��˳�����", "�˳�", MB_YESNO);
			if (res == IDYES) {
				delTrayIcon();
				PostQuitMessage(0);
			}
		}
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
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
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
	

	// 5. ��Ϣѭ��
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}