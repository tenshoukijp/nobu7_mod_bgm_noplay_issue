#include <windows.h>
#include <string>

#include "game_window.h"
#include "output_debug_stream.h"

using namespace std;

WNDPROC wpOrigWndProc = NULL;

// Subclass procedure 
LRESULT APIENTRY NB7WndProcCustom(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam)
{

	if (Msg == WM_DESTROY) {
		onDestroyWindow();
	}

	return CallWindowProc(wpOrigWndProc, hWnd, Msg, wParam, lParam);
}


// 画像系のハック
LRESULT Hook_DefWindowProcACustom(
	HWND hWnd,      // ウィンドウのハンドル
	UINT Msg,       // メッセージの識別子
	WPARAM wParam,  // メッセージの最初のパラメータ
	LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
	if (Msg == WM_CREATE) {

		char pszClassName[256] = { 0 };
		GetClassName(hWnd, pszClassName, _countof(pszClassName));
		if (string(pszClassName) == NB7_WINDOW_CLASS_NAME) {

			OutputDebugStream("WM_CREATE\n");
			CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lParam;
			OutputDebugStream(pCreateStruct->lpszClass);
			OutputDebugStream("\r\n");

			// ウィンドウ生成のタイミングで、ウィンドウプロシージャをこのMod内のもので指しはさむ
			wpOrigWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)NB7WndProcCustom);

			onCreateWindow(hWnd);
		}
	}

	return 0;
}




