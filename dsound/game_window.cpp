#include <windows.h>
#include "output_debug_stream.h"

HWND hNB7Wnd = NULL;

HWND* referenceOfNB7Wnd = NULL;

extern void hookFunctionsDirect();

void onCreateWindow(HWND hWnd) {

	// �E�B���h�E�n���h����ۑ�
	hNB7Wnd = hWnd;

	OutputDebugStream("�E�B���h�E����\n");
	OutputDebugStream("�����^�̊J�n\n");

	hookFunctionsDirect();
}

BOOL doneDestroyWindow = FALSE;
void onDestroyWindow() {
	if (!doneDestroyWindow) {
		OutputDebugStream("�E�B���h�E�j��\r\n");
	}

	doneDestroyWindow = TRUE;
}
