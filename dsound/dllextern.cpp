// dsound_Funcs.cpp : 基本デフォルトの処理へジャンプ。一部処理を挟む
//

#include <windows.h>
#include "dsound.h"

FARPROC p_DirectSoundCreate;
FARPROC p_DirectSoundEnumerateA;
FARPROC p_DirectSoundEnumerateW;
FARPROC p_DllCanUnloadNow;
FARPROC p_DllGetClassObject;
FARPROC p_DirectSoundCaptureCreate;
FARPROC p_DirectSoundCaptureEnumerateA;
FARPROC p_DirectSoundCaptureEnumerateW;
FARPROC p_GetDeviceID;
FARPROC p_DirectSoundFullDuplexCreate;
FARPROC p_DirectSoundCreate8;
FARPROC p_DirectSoundCaptureCreate8;

HINSTANCE hOriginalDll = NULL;;

__declspec( naked ) void WINAPI d_DirectSoundCreate() { _asm{ jmp p_DirectSoundCreate } }
__declspec( naked ) void WINAPI d_DirectSoundEnumerateA() { _asm{ jmp p_DirectSoundEnumerateA } }
__declspec( naked ) void WINAPI d_DirectSoundEnumerateW() { _asm{ jmp p_DirectSoundEnumerateW } }
__declspec( naked ) void WINAPI d_DllCanUnloadNow() { _asm{ jmp p_DllCanUnloadNow } }
__declspec( naked ) void WINAPI d_DllGetClassObject() { _asm{ jmp p_DllGetClassObject } }
__declspec( naked ) void WINAPI d_DirectSoundCaptureCreate() { _asm{ jmp p_DirectSoundCaptureCreate } }
__declspec( naked ) void WINAPI d_DirectSoundCaptureEnumerateA() { _asm{ jmp p_DirectSoundCaptureEnumerateA } }
__declspec( naked ) void WINAPI d_DirectSoundCaptureEnumerateW() { _asm{ jmp p_DirectSoundCaptureEnumerateW } }
__declspec( naked ) void WINAPI d_GetDeviceID() { _asm{ jmp p_GetDeviceID } }
__declspec( naked ) void WINAPI d_DirectSoundFullDuplexCreate() { _asm{ jmp p_DirectSoundFullDuplexCreate } }
__declspec( naked ) void WINAPI d_DirectSoundCreate8() { _asm{ jmp p_DirectSoundCreate8 } }
__declspec( naked ) void WINAPI d_DirectSoundCaptureCreate8() { _asm{ jmp p_DirectSoundCaptureCreate8 } }

void setDllFuncAddress() {
	p_DirectSoundCreate = GetProcAddress(hOriginalDll, "DirectSoundCreate");
	p_DirectSoundEnumerateA = GetProcAddress(hOriginalDll, "DirectSoundEnumerateA");
	p_DirectSoundEnumerateW = GetProcAddress(hOriginalDll, "DirectSoundEnumerateW");
	p_DllCanUnloadNow = GetProcAddress(hOriginalDll, "DllCanUnloadNow");
	p_DllGetClassObject = GetProcAddress(hOriginalDll, "DllGetClassObject");
	p_DirectSoundCaptureCreate = GetProcAddress(hOriginalDll, "DirectSoundCaptureCreate");
	p_DirectSoundCaptureEnumerateA = GetProcAddress(hOriginalDll, "DirectSoundCaptureEnumerateA");
	p_DirectSoundCaptureEnumerateW = GetProcAddress(hOriginalDll, "DirectSoundCaptureEnumerateW");
	p_GetDeviceID = GetProcAddress(hOriginalDll, "GetDeviceID");
	p_DirectSoundFullDuplexCreate = GetProcAddress(hOriginalDll, "DirectSoundFullDuplexCreate");
	p_DirectSoundCreate8 = GetProcAddress(hOriginalDll, "DirectSoundCreate8");
	p_DirectSoundCaptureCreate8 = GetProcAddress(hOriginalDll, "DirectSoundCaptureCreate8");
}