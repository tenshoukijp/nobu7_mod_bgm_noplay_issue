#include "windows.h"
#include "dllextern.h"
#include "process.h"

extern void hookFunctionsReplace();

void onInitialize() {
	hookFunctionsReplace();
}

void onFinalize() {
}