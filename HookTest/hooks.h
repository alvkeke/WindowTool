#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

#define FUNC_KEY VK_LMENU

int initHook(HINSTANCE hInstance, vector<string>);

void setListenState(bool foo);

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

