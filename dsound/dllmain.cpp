// dsound_DllMain.cpp : dsoundのエントリポイント

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
			MessageBox(NULL, "エラー", "「SysWow64」環境ではないようです。", NULL);
			return FALSE;
		}

		// C:\Windows\SysWow64のパスを取得。ほとんど全ての人はCドライブから変更していないとは思うが...
		char sysWow64Path[512] = "";
		UINT hasWow64Path = GetSystemWow64DirectoryA(sysWow64Path, sizeof(sysWow64Path));
		if (hasWow64Path == 0) {
			MessageBox(NULL, "エラー", "「SysWow64」のパスを取得できませんでした。", NULL);
			return FALSE;
		}
		strcat_s(sysWow64Path, "\\dsound.dll");

		hOriginalDll = LoadLibrary(sysWow64Path);
		if (hOriginalDll == NULL) {
			MessageBox(NULL, "エラー", "「SysWow64」内のdsound.dllをロードできませんでした。", NULL);
			return FALSE;
		}

		setDllFuncAddress();

		// 将星録.mod.dll が存在するならば、何もしない。将星録.mod.dllがこの機能を含むため。
		if (isFileExists("将星録.mod.dll")) {
			;
		}
		else {
			// プロセスのフルパスに「Nobunaga7WPK.exe」が含まれているか？
			std::string fullPathName = GetCurrentProcessFullPath();
			// 含まれているなら、将星録のexeが起動したということ。
				if (fullPathName.find("Nobunaga7WPK.exe") != std::string::npos) {
					// ここまで来たら「将星録本体」のexeが起動したということ。
					// ただし、これだけではまだ油断がならないのだ。
					// なぜなら、Nobunaga7WPK.exeをクリックしても、「ランチャーの方にリダイレクトする」という機能が将星録Steam版にはついているため。
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
