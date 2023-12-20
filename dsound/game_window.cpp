#include <windows.h>
#include "output_debug_stream.h"

HWND hNB7Wnd = NULL;

HWND* referenceOfNB7Wnd = NULL;

extern void hookFunctionsDirect();

void onCreateWindow(HWND hWnd) {

	// ウィンドウハンドルを保存
	hNB7Wnd = hWnd;

	OutputDebugStream("ウィンドウ生成\n");
	OutputDebugStream("将星録の開始\n");

	hookFunctionsDirect();
}

BOOL doneDestroyWindow = FALSE;
void onDestroyWindow() {
	if (!doneDestroyWindow) {
		OutputDebugStream("ウィンドウ破棄\r\n");
	}

	doneDestroyWindow = TRUE;
}
