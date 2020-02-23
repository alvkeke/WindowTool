#include <Windows.h>
#include <vector>
#include <string>


using namespace std;

#define HOOK_KEY_FUNC VK_LMENU
#define HOT_KEY_FUNC MOD_ALT
#define HOOK_KEY_SWITCH VK_F1


#define WM_CALLBACK_DISABLE WM_USER + 2

int initHook(HINSTANCE hInstance, vector<string>);

void setListenState(bool foo);

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

