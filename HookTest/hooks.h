#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

int initHook(HINSTANCE hInstance, vector<string>);

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);