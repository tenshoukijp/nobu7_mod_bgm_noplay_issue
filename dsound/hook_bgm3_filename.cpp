#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"

using namespace std;


/*
// �푈���I��������Ȃǂ͂�����ʉ߂���͗l�B
00419300   81EC 04010000    SUB ESP,104
00419306   A1 10EE5300      MOV EAX,DWORD PTR DS:[53EE10]
0041930B   33C4             XOR EAX,ESP
0041930D   898424 00010000  MOV DWORD PTR SS:[ESP+100],EAX
00419314   8B9424 10010000  MOV EDX,DWORD PTR SS:[ESP+110]
0041931B   53               PUSH EBX
0041931C   56               PUSH ESI
0041931D   33DB             XOR EBX,EBX
0041931F   3BD3             CMP EDX,EBX
00419321   57               PUSH EDI
00419322   8BBC24 14010000  MOV EDI,DWORD PTR SS:[ESP+114]
00419329   8BF1             MOV ESI,ECX
0041932B   8B8C24 18010000  MOV ECX,DWORD PTR SS:[ESP+118]
00419332   74 08            JE SHORT Nobunaga.0041933C
00419334   897E 1C          MOV DWORD PTR DS:[ESI+1C],EDI
00419337   894E 24          MOV DWORD PTR DS:[ESI+24],ECX
0041933A   EB 10            JMP SHORT Nobunaga.0041934C
0041933C   897E 18          MOV DWORD PTR DS:[ESI+18],EDI
0041933F   894E 20          MOV DWORD PTR DS:[ESI+20],ECX
00419342   C746 1C FFFFFFFF MOV DWORD PTR DS:[ESI+1C],-1
00419349   895E 24          MOV DWORD PTR DS:[ESI+24],EBX
0041934C   395E 0C          CMP DWORD PTR DS:[ESI+C],EBX
0041934F   74 66            JE SHORT Nobunaga.004193B7
00419351   8B06             MOV EAX,DWORD PTR DS:[ESI]
00419353   2BC3             SUB EAX,EBX
00419355   74 54            JE SHORT Nobunaga.004193AB
00419357   83E8 01          SUB EAX,1
0041935A   75 62            JNZ SHORT Nobunaga.004193BE
0041935C   8B46 2C          MOV EAX,DWORD PTR DS:[ESI+2C]
0041935F   3BC3             CMP EAX,EBX
00419361   74 0D            JE SHORT Nobunaga.00419370
00419363   50               PUSH EAX
00419364   FF15 C4005200    CALL DWORD PTR DS:[<&KDvdDSnd.DSoundClos>; KDvdDSnd.DSoundClose
0041936A   83C4 04          ADD ESP,4
0041936D   895E 2C          MOV DWORD PTR DS:[ESI+2C],EBX
00419370   8D4424 0C        LEA EAX,DWORD PTR SS:[ESP+C]
00419374   57               PUSH EDI
00419375   50               PUSH EAX
00419376   E8 C5FEFFFF      CALL Nobunaga.00419240
0041937B   83C4 08          ADD ESP,8
0041937E   83FF FF          CMP EDI,-1
00419381   74 3B            JE SHORT Nobunaga.004193BE
00419383   8B56 30          MOV EDX,DWORD PTR DS:[ESI+30]
00419386   53               PUSH EBX
00419387   8D4C24 10        LEA ECX,DWORD PTR SS:[ESP+10]
0041938B   51               PUSH ECX
0041938C   52               PUSH EDX
0041938D   FF15 D8005200    CALL DWORD PTR DS:[<&KDvdDSnd.DSoundOpen>; KDvdDSnd.DSoundOpen
00419393   83C4 0C          ADD ESP,0C
00419396   3BC3             CMP EAX,EBX
00419398   8946 2C          MOV DWORD PTR DS:[ESI+2C],EAX
0041939B   74 21            JE SHORT Nobunaga.004193BE
0041939D   6A 01            PUSH 1
0041939F   50               PUSH EAX
004193A0   FF15 E0005200    CALL DWORD PTR DS:[<&KDvdDSnd.DSoundPlay>; KDvdDSnd.DSoundPlay
004193A6   83C4 08          ADD ESP,8
004193A9   EB 15            JMP SHORT Nobunaga.004193C0
004193AB   52               PUSH EDX
004193AC   51               PUSH ECX
004193AD   57               PUSH EDI
004193AE   8BCE             MOV ECX,ESI
004193B0   E8 0BFDFFFF      CALL Nobunaga.004190C0
004193B5   EB 09            JMP SHORT Nobunaga.004193C0
004193B7   B8 01000000      MOV EAX,1
004193BC   EB 02            JMP SHORT Nobunaga.004193C0
004193BE   8BC3             MOV EAX,EBX
004193C0   8B8C24 0C010000  MOV ECX,DWORD PTR SS:[ESP+10C]
004193C7   5F               POP EDI
004193C8   5E               POP ESI
004193C9   5B               POP EBX
004193CA   33CC             XOR ECX,ESP
004193CC   E8 B6000F00      CALL Nobunaga.00509487
004193D1   81C4 04010000    ADD ESP,104
004193D7   C2 0C00          RETN 0C

*/


static int nPlayBGM3EaxBackup = -1;
static int nPlayBGM3Arg1 = -1;
static int nPlayBGM3Arg2 = -1;
static int nPlayBGM3Arg3 = -1;

extern int bgm_noloop_issue_patch(int iBGMNo);

void OnSSRExePlayBGM3Execute() {
	OutputDebugStream("BGM3�̍Đ��֐����Ă΂ꂽ��!!\n");
	OutputDebugStream("BGM3%d, %d, �ԍ�%d\n", nPlayBGM3Arg1, nPlayBGM3Arg2, nPlayBGM3Arg3);

	// �헪��ʂȂǂŁA���̃V�[���ɍs���Ė߂��Ă����BGM���ĊJ���Ȃ��Ƃ����o�O�̏C��
	nPlayBGM3Arg3 = bgm_noloop_issue_patch(nPlayBGM3Arg3);

	int iBGMID = nPlayBGM3Arg3; // ���ꂪ�Đ������BGM��ID;
}



/*
��ʓI��BGM�̕�(��1�̕�)��BGM�Đ��֐��Ăяo���t��
00419300   81EC 04010000    SUB ESP,104
00419306   A1 10EE5300      MOV EAX,DWORD PTR DS:[53EE10]
0041930B   33C4             XOR EAX,ESP
*/

int pSSRExeJumpFromToOnSSRExePlayBGM3 = 0x00419300; // �֐��͂��̃A�h���X����AOnSSRExePlayBGM3�ւƃW�����v���Ă���B
int pSSRExeReturnLblFromOnSSRExePlayBGM3 = 0x419306; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExePlayBGM3() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov nPlayBGM3EaxBackup, eax          // ��ŕ������邽�߁A�����EAX�͎���Ă���

		MOV eax, DWORD PTR SS : [ESP + 0x4] // 3�Ԗڂ̈������擾
		mov nPlayBGM3Arg3, eax
		MOV eax, DWORD PTR SS : [ESP + 0x8] // 2�Ԗڂ̈������擾
		mov nPlayBGM3Arg2, eax
		MOV eax, DWORD PTR SS : [ESP + 0xC] // 1�Ԗڂ̈������擾
		mov nPlayBGM3Arg1, eax

		mov eax, nPlayBGM3EaxBackup		     // EAX�𕜌�����

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExePlayBGM3Execute();

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

		MOV nPlayBGM3EaxBackup, EAX
		MOV EAX, nPlayBGM3Arg3
		MOV DWORD PTR SS : [ESP + 0x4] , EAX // ���C����ʂɖ߂��Ă������ABGM����Ȃ����Ƃ�����̂Ńp�b�`
		MOV EAX, nPlayBGM3EaxBackup

		SUB ESP, 0x104   // ���̋L�q��������

		jmp pSSRExeReturnLblFromOnSSRExePlayBGM3
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromPlayBGM3[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�

// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExePlayBGM3() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExePlayBGM3;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExePlayBGM3 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromPlayBGM3 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExePlayBGM3), cmdOnSSRExeJumpFromPlayBGM3, 5, NULL); //5�o�C�g�̂ݏ�������
}

