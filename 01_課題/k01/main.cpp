// �w�b�_�[
#include "main.h"

#include <iostream>
#include <cstdio>
#include "strsafe.h"

#include "Window.h"
#include "Window2.h"

// �O���[�o���ϐ�
HINSTANCE g_hInstance;

// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	Window window(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window.Create() == FALSE) {
		return 0;
	}
	//HWND hWnd = window.GetWinHandle();
	//SetWindowPos(hWnd, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
	
	//Window2 window2(TEXT("window2"), SCREEN_WIDTH, SCREEN_HEIGHT);
	//if (window2.Create() == FALSE) {
	//	return 0;
	//}
	//HWND hWnd2 = window2.GetWinHandle();
	//SetWindowPos(hWnd2, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);



	MSG msg;
	// ���b�Z�[�W���[�v
	msg.message = WM_NULL;
	while (WM_QUIT != msg.message) {					// WM_QUIT ������܂Ń��[�v
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {	// ���b�Z�[�W���`�F�b�N
			TranslateMessage(&msg);						// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
			DispatchMessage(&msg);						// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
		} else {

		}
	}
	return msg.wParam;
}

