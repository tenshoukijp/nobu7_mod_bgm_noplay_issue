#pragma once

#include <windows.h>

extern HWND hNB7Wnd;

extern HWND* referenceOfNB7Wnd;

#define NB7_WINDOW_CLASS_NAME "信長の野望･将星録 パワーアップキット"

void onCreateWindow(HWND hWnd);

void onDestroyWindow();