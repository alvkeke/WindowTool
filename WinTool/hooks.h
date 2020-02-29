#include <Windows.h>
#include <vector>
#include <string>


using namespace std;

#define HOOK_KEY_FUNC VK_LWIN
#define HOOK_KEY_SWITCH VK_F1
#define HOOK_KEY_UP 'I'
#define HOOK_KEY_DOWN 'K'
#define HOOK_KEY_LEFT 'J'
#define HOOK_KEY_RIGHT 'L'

#define WM_CALLBACK_SWITCH WM_USER + 2

int initHook(HINSTANCE hInstance, HWND hwnd, vector<string>);

void setListenState(bool foo);

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

