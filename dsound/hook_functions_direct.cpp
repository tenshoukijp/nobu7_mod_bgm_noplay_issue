extern void WriteAsmJumperOnSSRExePlayBGM();
extern void WriteAsmJumperOnSSRExePlayBGM3();


bool isWriteAsmJumperOnSSRExePlayBGM = false;
bool isWriteAsmJumperOnSSRExePlayBGM3 = false;


void hookFunctionsDirect() {

	if (!isWriteAsmJumperOnSSRExePlayBGM) {
		isWriteAsmJumperOnSSRExePlayBGM = true;
		WriteAsmJumperOnSSRExePlayBGM();
	}

	if (!isWriteAsmJumperOnSSRExePlayBGM3) {
		isWriteAsmJumperOnSSRExePlayBGM3 = true;
		WriteAsmJumperOnSSRExePlayBGM3();
	}
}