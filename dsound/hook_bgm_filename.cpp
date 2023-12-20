#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

using namespace std;


/*
// ��ʓI��BGM�͂���(��1)
00419240   8B4424 08        MOV EAX,DWORD PTR SS:[ESP+8]
00419244   8D48 FE          LEA ECX,DWORD PTR DS:[EAX-2]
00419247   83F9 12          CMP ECX,12                                0x12 = 18 �Ƃ�������͓�BBGM��BGM\BGM9.wav �Ȃǂ����ʂɂ��邵...
0041924A   77 14            JA SHORT Nobunaga.00419260
0041924C   8B5424 04        MOV EDX,DWORD PTR SS:[ESP+4]
00419250   50               PUSH EAX
00419251   68 4C0F5200      PUSH Nobunaga.00520F4C                   ; ASCII "BGM\%02d.wav"
00419256   52               PUSH EDX
00419257   E8 1B950E00      CALL Nobunaga.00502777
0041925C   83C4 0C          ADD ESP,0C
0041925F   C3               RETN


/*
��ʓI��BGM�̕�(��1�̕�)��BGM�Đ��֐��Ăяo���t��
00419374   57               PUSH EDI
00419375   50               PUSH EAX
00419376   E8 C5FEFFFF      CALL Nobunaga.00419240
0041937B   83C4 08          ADD ESP,8

������̏�̕��ŁA�����̊֐��Ăяo���͖c�� (���� 0x4193E0��BGM�Đ��֐�)��
004193E0   81EC 04010000    SUB ESP,104
004193E6   A1 10EE5300      MOV EAX,DWORD PTR DS:[53EE10]
004193EB   33C4             XOR EAX,ESP
004193ED   898424 00010000  MOV DWORD PTR SS:[ESP+100],EAX
004193F4   53               PUSH EBX
004193F5   55               PUSH EBP
004193F6   8BAC24 14010000  MOV EBP,DWORD PTR SS:[ESP+114]
004193FD   56               PUSH ESI
004193FE   8BF1             MOV ESI,ECX
00419400   83CB FF          OR EBX,FFFFFFFF


����  CALL Nobunaga.004193E0 ��������uBGM�Đ��֐��v�̖͗l�B
0044EA66   6A 00            PUSH 0
0044EA68   6A 01            PUSH 1
0044EA6A   50               PUSH EAX
0044EA6B   B9 A0815300      MOV ECX,Nobunaga.005381A0   0x5381A0 �͌Œ�Ȗ͗l
0044EA70   E8 6BA9FCFF      CALL Nobunaga.004193E0
*/


static int nPlayBgmEaxBackup = -1;
static int nPlayBgmArg1 = -1;
static int nPlayBgmArg2 = -1;
static int nPlayBgmArg3 = -1;

extern int bgm_noloop_issue_patch(int iBGMNo);

void OnSSRExePlayBGMExecute() {
	OutputDebugStream("BGM�̍Đ��֐����Ă΂ꂽ��!!\n");
	OutputDebugStream("BGM%d, %d, �ԍ�%d\n", nPlayBgmArg1, nPlayBgmArg2, nPlayBgmArg3);

	// �O����ʂȂǂŁA���̃V�[���ɍs���Ė߂��Ă����BGM���ĊJ���Ȃ��Ƃ����o�O�̏C��
	nPlayBgmArg3 = bgm_noloop_issue_patch(nPlayBgmArg3);

	int iBGMID = nPlayBgmArg3; // ���ꂪ�Đ������BGM��ID;
}



/*
��ʓI��BGM�̕�(��1�̕�)��BGM�Đ��֐��Ăяo���t��
004193E0   81EC 04010000    SUB ESP,104
004193E6   A1 10EE5300      MOV EAX,DWORD PTR DS:[53EE10]
004193EB   33C4             XOR EAX,ESP
*/

int pSSRExeJumpFromToOnSSRExePlayBGM = 0x4193E0; // �֐��͂��̃A�h���X����AOnSSRExePlayBGM�ւƃW�����v���Ă���B
int pSSRExeReturnLblFromOnSSRExePlayBGM = 0x4193E6; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExePlayBGM() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov nPlayBgmEaxBackup, eax          // ��ŕ������邽�߁A�����EAX�͎���Ă���

		MOV eax, DWORD PTR SS : [ESP + 0x4] // 3�Ԗڂ̈������擾
		mov nPlayBgmArg3, eax
		MOV eax, DWORD PTR SS : [ESP + 0x8] // 2�Ԗڂ̈������擾
		mov nPlayBgmArg2, eax
		MOV eax, DWORD PTR SS : [ESP + 0xC] // 1�Ԗڂ̈������擾
		mov nPlayBgmArg1, eax

		mov eax, nPlayBgmEaxBackup		     // EAX�𕜌�����

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExePlayBGMExecute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		MOV nPlayBgmEaxBackup, EAX
		MOV EAX, nPlayBgmArg3
		MOV DWORD PTR SS : [ESP + 0x4], EAX // ���C����ʂɖ߂��Ă������ABGM����Ȃ����Ƃ�����̂Ńp�b�`
		MOV EAX, nPlayBgmEaxBackup

		SUB ESP, 0x104   // ���̋L�q��������

		jmp pSSRExeReturnLblFromOnSSRExePlayBGM
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromPlayBGM[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�

// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExePlayBGM() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExePlayBGM;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExePlayBGM + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromPlayBGM + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExePlayBGM), cmdOnSSRExeJumpFromPlayBGM, 5, NULL); //5�o�C�g�̂ݏ�������
}

