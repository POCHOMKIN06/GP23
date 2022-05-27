//���K�@�x�N�g���̕\��

// �w�b�_�[
#include "main.h"

#include <iostream>
#include <cstdio>
#include "strsafe.h"


#include "AxisLine.h"
#include "VIewer.h"
#include "MoveType.h"
#include "MovingCircle.h"
#include "Vector.h"

//�}�N����`
#define IDT_TIMER1	(100)

// �v���g�^�C�v�錾
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);

//�p�����[�^�v���Z�b�g
//constexpr float INIT_VEL     = 10.0f;
//constexpr float ACCELERATION = 4.0f;
//constexpr float GRAVITY      = -9.8f;

// �O���[�o���ϐ�
HINSTANCE g_hInstance;
//float g_time;
//float g_initvel;
//float g_acc;
//float g_gravityAcc;
//float g_theta;
POINT g_origin = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

//����I�u�W�F�N�g
AxisLine		g_AxisLine(g_origin.x, g_origin.y);		//���W��
//Viewer			g_Viewer;								//�e��p�����[�^�\��
//MOVE_TYPE		g_MoveType = MOVE_TYPE::Constant_Velocity;				//�^�������̎��
//MovingCircle	g_Circle[static_cast<unsigned int>(MOVE_TYPE::END)];	//�^������~
Vector			g_Vector[10];


// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;


	WNDCLASSEX wcex = {
		sizeof( WNDCLASSEX ),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor( NULL, IDC_ARROW ),	//�J�[�\���A�C�R���Ǎ�
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};

	HWND	hWnd;
	MSG		msg;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,	//�쐬�ʒu(x,y)
		CW_USEDEFAULT,	//����(0,0)
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//��3����:50ms(1000/20fps)

	// ���b�Z�[�W���[�v
	while (GetMessage(&msg, NULL, 0, 0 ) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// �E�B���h�E�̓o�^��j��
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam)
{
	//�W���I�u�W�F�N�g
	HDC hDC;			//�f�o�C�X�R���e�L�X�g�̃n���h��
	PAINTSTRUCT ps;		//�N���C�A���g�̈�ɕ`�悷�鎞�ɕK�v�ȍ\����
	HPEN hPen;			//�y���I�u�W�F�N�g
	HPEN hPenOld;		//�ύX�O�̃I�u�W�F�N�g�ۑ��p
	HBRUSH hBrush;		//�u���V�I�u�W�F�N�g
	HBRUSH hBrushOld;	//�ύX�O�̃I�u�W�F�N�g�ۑ��p


	switch(uMsg)
	{
	case WM_DESTROY:	// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		g_Vector[0] = Vector(60, 90);
		break;
	case WM_TIMER:
	{
		//�ĕ`����w��
		RECT rect = {
			0,0, SCREEN_WIDTH, SCREEN_HEIGHT
		};
		InvalidateRect(hWnd, &rect, TRUE);

		break;
	}
	case WM_PAINT:		//�`�施�߂��o��
	{
		//�`��J�n
		hDC = BeginPaint(hWnd, &ps);

		//
		//�`�揈���͂����ɏ���
		//

		//���W���쐬
		g_AxisLine.Draw(hDC);
		//�\����
		g_Vector[0].Draw(hDC, g_origin);


		//�y�����쐬
		hPen = CreatePen(PS_SOLID, 3, GREEN);	//����,����,�F
		//�y���������ւ�
		hPenOld = (HPEN)SelectObject(hDC, hPen);

		//�~��`��

		//�y����߂�
		SelectObject(hDC, hPenOld);
		//�쐬�����y���̍폜
		DeleteObject(hPen);

		//�u���V�̍쐬
		hBrush = CreateSolidBrush(RED);
		//�u���V�������ւ�
		hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);

		//�l�p��`��
		//Rectangle(hDC, 340, 340, 460, 460);
		//�u���V��߂�
		SelectObject(hDC, hBrushOld);
		//�쐬�����u���V�̍폜
		DeleteObject(hBrush);

		//�`��I��
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:	//�L�[���������ꂽ
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			int id = MessageBox(hWnd,
				TEXT("�I�����܂���?"),
				TEXT("�m�F"),
				MB_YESNO);
			if (id == IDYES) {
				DestroyWindow(hWnd);
			}
		}
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		case VK_RIGHT:
			break;
		case VK_LEFT:
			break;
		case VK_NUMPAD1:
			break;
		case VK_NUMPAD2:
			break;
		case VK_NUMPAD3:
			break;
		case VK_NUMPAD4:
			break;
		case VK_NUMPAD5:
			break;
		case VK_SPACE:
			break;
		default:
			break;
		}
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

