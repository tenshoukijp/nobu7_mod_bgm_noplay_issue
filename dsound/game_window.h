#pragma once

#include <windows.h>

extern HWND hNB7Wnd;

extern HWND* referenceOfNB7Wnd;

#define NB7_WINDOW_CLASS_NAME "�M���̖�]������^ �p���[�A�b�v�L�b�g"

void onCreateWindow(HWND hWnd);

void onDestroyWindow();