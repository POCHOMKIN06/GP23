//�ۑ�05�@�^�����Ă�?
//2:�������x�^���A3:���R�����^���A5:�Ε����˂���ʓ��ɕ\������
//��o����:5/13(��)

// �w�b�_�[
#include "main.h"

#include <iostream>
#include <cstdio>
#include "strsafe.h"

#include "Window.h"




//�}�N����`
#define IDT_TIMER1	(100)

// �v���g�^�C�v�錾

// �O���[�o���ϐ�
HINSTANCE g_hInstance;

using namespace Oshima;

// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	Window window(Window::ClassName, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window.Create() == FALSE) {
		return 0;
	}

	HWND	hWnd = window.GetWinHandle();
	MSG		msg;

	SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//��3����:50ms(1000/20fps)

	// ���b�Z�[�W���[�v
 	while (GetMessage(&msg, NULL, 0, 0 ) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
