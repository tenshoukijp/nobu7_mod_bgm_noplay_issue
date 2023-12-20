#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <string>
#include <algorithm>
#include <cctype>

#include "game_process.h"

// ImageDirectoryEntryToData
#pragma comment(lib, "dbghelp.lib")


// �ЂƂ̃��W���[���ɑ΂���API�t�b�N���s���֐�
void ReplaceIATEntryInOneMod(
    PCSTR pszModuleName,
    PROC pfnCurrent,
    PROC pfnNew,
    HMODULE hmodCaller)
{
    ULONG ulSize;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
        hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

    if (pImportDesc == NULL)
        return;

    while (pImportDesc->Name) {
        PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);
        if (lstrcmpiA(pszModName, pszModuleName) == 0)
            break;
        pImportDesc++;
    }

    if (pImportDesc->Name == 0)
        return;

    PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
        ((PBYTE)hmodCaller + pImportDesc->FirstThunk);

    while (pThunk->u1.Function) {
        PROC* ppfn = (PROC*)&pThunk->u1.Function;
        BOOL fFound = (*ppfn == pfnCurrent);
        if (fFound) {
            DWORD dwDummy;
            VirtualProtect(ppfn, sizeof(ppfn), PAGE_EXECUTE_READWRITE, &dwDummy);
            WriteProcessMemory(
                hCurrentProcess, ppfn, &pfnNew, sizeof(pfnNew), NULL);
            return;
        }
        pThunk++;
    }
    return;
}

// ���ׂẴ��W���[���ɑ΂���API�t�b�N���s���֐�
void ReplaceIATEntryInAllMods(
    PCSTR pszModuleName,
    PROC pfnCurrent,
    PROC pfnNew)
{
    // ���W���[�����X�g���擾
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hModuleSnap == INVALID_HANDLE_VALUE)
        return;

    MODULEENTRY32 me;
    me.dwSize = sizeof(me);
    BOOL bModuleResult = Module32First(hModuleSnap, &me);
    // ���ꂼ��̃��W���[���ɑ΂���ReplaceIATEntryInOneMod�����s
    while (bModuleResult) {
        ReplaceIATEntryInOneMod(pszModuleName, pfnCurrent, pfnNew, me.hModule);
        bModuleResult = Module32Next(hModuleSnap, &me);
    }
    CloseHandle(hModuleSnap);
}

//---------------------------WindowProcA

// �t�b�N����֐��̃v���g�^�C�v���`
using PFNDEFWINDOWPROCA = LRESULT(WINAPI*)(HWND, UINT, WPARAM, LPARAM);

PROC pfnOrigDefWindowProcA = GetProcAddress(GetModuleHandleA("user32.dll"), "DefWindowProcA");

extern LRESULT Hook_DefWindowProcACustom(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI Hook_DefWindowProcA(
    HWND hWnd,      // �E�B���h�E�̃n���h��
    UINT Msg,       // ���b�Z�[�W�̎��ʎq
    WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
    LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
{
    Hook_DefWindowProcACustom(hWnd, Msg, wParam, lParam);

    // �X�N���[���Z�C�o�[�̋N����h�~����B�X�N���[���Z�C�o�[�ɂȂ肻���Ȏ��ɁA�N�����Ă���A�v���̂����ꂩ���A1��Ԃ���
    // �X�N���[���Z�C�o�[�̋N���͖h�~�����悤���B
    if (Msg == WM_SYSCOMMAND && wParam == SC_SCREENSAVE) {
        // �X�N���[���Z�C�o�[�͋N�����Ȃ��B
        return 1;
    }

    //-------------------------------------------- �������牺�̓I���W�i�����Ăяo������
    // ���̌�A���̂��̂��Ăяo��
    LRESULT LPResult = ((PFNDEFWINDOWPROCA)pfnOrigDefWindowProcA)(hWnd, Msg, wParam, lParam);

    return LPResult;
}



/*----------------------------------------------------------------*
 HOOK�n����
 *----------------------------------------------------------------*/
bool isHookDefWindowProcA = false;

void hookFunctionsReplace() {

    PROC pfnOrig;
    if (!isHookDefWindowProcA) {
        isHookDefWindowProcA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "DefWindowProcA");
        ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_DefWindowProcA);
    }
}