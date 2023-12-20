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


// �摜�n�̃n�b�N
LRESULT Hook_DefWindowProcACustom(
	HWND hWnd,      // �E�B���h�E�̃n���h��
	UINT Msg,       // ���b�Z�[�W�̎��ʎq
	WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
	LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
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

			// �E�B���h�E�����̃^�C�~���O�ŁA�E�B���h�E�v���V�[�W��������Mod���̂��̂Ŏw���͂���
			wpOrigWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)NB7WndProcCustom);

			onCreateWindow(hWnd);
		}
	}

	return 0;
}




