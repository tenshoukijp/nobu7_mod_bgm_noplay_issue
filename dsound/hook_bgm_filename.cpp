#include <windows.h>
#include <string>
#include "game_process.h"
#include "output_debug_stream.h"

using namespace std;


/*
// 一般的なBGMはここ(※1)
00419240   8B4424 08        MOV EAX,DWORD PTR SS:[ESP+8]
00419244   8D48 FE          LEA ECX,DWORD PTR DS:[EAX-2]
00419247   83F9 12          CMP ECX,12                                0x12 = 18 という縛りは謎。BGMはBGM\BGM9.wav なども普通にあるし...
0041924A   77 14            JA SHORT Nobunaga.00419260
0041924C   8B5424 04        MOV EDX,DWORD PTR SS:[ESP+4]
00419250   50               PUSH EAX
00419251   68 4C0F5200      PUSH Nobunaga.00520F4C                   ; ASCII "BGM\%02d.wav"
00419256   52               PUSH EDX
00419257   E8 1B950E00      CALL Nobunaga.00502777
0041925C   83C4 0C          ADD ESP,0C
0041925F   C3               RETN


/*
一般的なBGMの方(※1の方)のBGM再生関数呼び出し付近
00419374   57               PUSH EDI
00419375   50               PUSH EAX
00419376   E8 C5FEFFFF      CALL Nobunaga.00419240
0041937B   83C4 08          ADD ESP,8

★これの上の方で、ここの関数呼び出しは膨大 (この 0x4193E0がBGM再生関数)★
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


この  CALL Nobunaga.004193E0 がいわゆる「BGM再生関数」の模様。
0044EA66   6A 00            PUSH 0
0044EA68   6A 01            PUSH 1
0044EA6A   50               PUSH EAX
0044EA6B   B9 A0815300      MOV ECX,Nobunaga.005381A0   0x5381A0 は固定な模様
0044EA70   E8 6BA9FCFF      CALL Nobunaga.004193E0
*/


static int nPlayBgmEaxBackup = -1;
static int nPlayBgmArg1 = -1;
static int nPlayBgmArg2 = -1;
static int nPlayBgmArg3 = -1;

extern int bgm_noloop_issue_patch(int iBGMNo);

void OnSSRExePlayBGMExecute() {
	OutputDebugStream("BGMの再生関数が呼ばれたよ!!\n");
	OutputDebugStream("BGM%d, %d, 番号%d\n", nPlayBgmArg1, nPlayBgmArg2, nPlayBgmArg3);

	// 外交画面などで、他のシーンに行って戻ってくるとBGMが再開しないというバグの修正
	nPlayBgmArg3 = bgm_noloop_issue_patch(nPlayBgmArg3);

	int iBGMID = nPlayBgmArg3; // これが再生されるBGMのID;
}



/*
一般的なBGMの方(※1の方)のBGM再生関数呼び出し付近
004193E0   81EC 04010000    SUB ESP,104
004193E6   A1 10EE5300      MOV EAX,DWORD PTR DS:[53EE10]
004193EB   33C4             XOR EAX,ESP
*/

int pSSRExeJumpFromToOnSSRExePlayBGM = 0x4193E0; // 関数はこのアドレスから、OnSSRExePlayBGMへとジャンプしてくる。
int pSSRExeReturnLblFromOnSSRExePlayBGM = 0x4193E6; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExePlayBGM() {
	// スタックにためておく
	__asm {
		mov nPlayBgmEaxBackup, eax          // 後で復元するため、現状のEAXは取っておく

		MOV eax, DWORD PTR SS : [ESP + 0x4] // 3番目の引数を取得
		mov nPlayBgmArg3, eax
		MOV eax, DWORD PTR SS : [ESP + 0x8] // 2番目の引数を取得
		mov nPlayBgmArg2, eax
		MOV eax, DWORD PTR SS : [ESP + 0xC] // 1番目の引数を取得
		mov nPlayBgmArg1, eax

		mov eax, nPlayBgmEaxBackup		     // EAXを復元する

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

	// スタックに引き出す
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
		MOV DWORD PTR SS : [ESP + 0x4], EAX // メイン画面に戻ってきた時、BGMが鳴らないことがあるのでパッチ
		MOV EAX, nPlayBgmEaxBackup

		SUB ESP, 0x104   // 元の記述をここに

		jmp pSSRExeReturnLblFromOnSSRExePlayBGM
	}
}
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromPlayBGM[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない

// ニーモニック書き換え用
void WriteAsmJumperOnSSRExePlayBGM() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExePlayBGM;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExePlayBGM + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromPlayBGM + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExePlayBGM), cmdOnSSRExeJumpFromPlayBGM, 5, NULL); //5バイトのみ書き込む
}

