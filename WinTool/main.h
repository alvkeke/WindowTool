#pragma once
#include <fstream>
#include <string>
#include <vector>


#define FILENAME			"MarkConfigure.conf"
#define WNDCLASS			"ALV_KEYTOOL_WND"
#define WNDTITLE			"������ݲ�������"

#define TITLE_CB_LISTEN		"���ÿ�ݲ���"

#define HELP_TEXT	"������\n������ͨ����win������������ʵ�ָ��ֹ��ܣ����й��ܶ��ڡ�win���������µ�����½��С����й������£�\n1.�������϶����ƶ�����\n2.����Ҽ��϶������Ŵ���\n3.����м����£����/С������\n4.���������ϣ��ö�����\n5.���������£�ȡ�������ö�"

/* �����ǿؼ�ID���������ڲ˵����� */
#define ID_CB_LISTEN		10001
#define ID_CB_TIPS			10002

#define ID_LB_KEYDOWN		10004

using namespace std;

void showPopupMenu(HWND hwnd, HMENU hmPop);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

