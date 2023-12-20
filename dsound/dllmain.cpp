// dsound_DllMain.cpp : dsound�̃G���g���|�C���g

#include <windows.h>
#include "process.h"
#include "dllextern.h"
#include "fileexist.h"

extern void setDllFuncAddress();
extern void onInitialize();

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved ) {

	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
	{
		if (!IsWow64()) {
			MessageBox(NULL, "�G���[", "�uSysWow64�v���ł͂Ȃ��悤�ł��B", NULL);
			return FALSE;
		}

		// C:\Windows\SysWow64�̃p�X���擾�B�قƂ�ǑS�Ă̐l��C�h���C�u����ύX���Ă��Ȃ��Ƃ͎v����...
		char sysWow64Path[512] = "";
		UINT hasWow64Path = GetSystemWow64DirectoryA(sysWow64Path, sizeof(sysWow64Path));
		if (hasWow64Path == 0) {
			MessageBox(NULL, "�G���[", "�uSysWow64�v�̃p�X���擾�ł��܂���ł����B", NULL);
			return FALSE;
		}
		strcat_s(sysWow64Path, "\\dsound.dll");

		hOriginalDll = LoadLibrary(sysWow64Path);
		if (hOriginalDll == NULL) {
			MessageBox(NULL, "�G���[", "�uSysWow64�v����dsound.dll�����[�h�ł��܂���ł����B", NULL);
			return FALSE;
		}

		setDllFuncAddress();

		// �����^.mod.dll �����݂���Ȃ�΁A�������Ȃ��B�����^.mod.dll�����̋@�\���܂ނ��߁B
		if (isFileExists("�����^.mod.dll")) {
			;
		}
		else {
			// �v���Z�X�̃t���p�X�ɁuNobunaga7WPK.exe�v���܂܂�Ă��邩�H
			std::string fullPathName = GetCurrentProcessFullPath();
			// �܂܂�Ă���Ȃ�A�����^��exe���N�������Ƃ������ƁB
				if (fullPathName.find("Nobunaga7WPK.exe") != std::string::npos) {
					// �����܂ŗ�����u�����^�{�́v��exe���N�������Ƃ������ƁB
					// �������A���ꂾ���ł͂܂����f���Ȃ�Ȃ��̂��B
					// �Ȃ��Ȃ�ANobunaga7WPK.exe���N���b�N���Ă��A�u�����`���[�̕��Ƀ��_�C���N�g����v�Ƃ����@�\�������^Steam�łɂ͂��Ă��邽�߁B
					onInitialize();
				}
		}

		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if (hOriginalDll) { FreeLibrary(hOriginalDll); }
		break;
	default:
		break;
	}
	return TRUE;
}
