//�ۑ�05�@�^�����Ă�?
//2:�������x�^���A3:���R�����^���A5:�Ε����˂���ʓ��ɕ\������
//��o����:5/13(��)

// �w�b�_�[
#include "main.h"

#include <iostream>
#include <cstdio>
#include "strsafe.h"


#include "AxisLine.h"
#include "VIewer.h"
#include "MoveType.h"
#include "MovingCircle.h"

//�}�N����`
#define IDT_TIMER1 (100)

// �v���g�^�C�v�錾
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(void);
void TimeReset(void);
void UpdateMoveType(MOVE_TYPE type);

////�p�����[�^�v���Z�b�g
//constexpr float INIT_VEL     = 10.0f;
//constexpr float ACCELERATION = 5.0f;
//constexpr float GRAVITY      = -9.8f;

// �O���[�o���ϐ�
HINSTANCE g_hInstance;
float g_time;
//float g_initvel;
//float g_acc;
//float g_gravityAcc;
//float g_theta;
POINT g_origin = { 200, SCREEN_HEIGHT / 2 };

//����I�u�W�F�N�g
AxisLine		g_AxisLine(g_origin.x, g_origin.y);		//���W��
Viewer			g_Viewer;								//�e��p�����[�^�\��
MovingCircle	g_Circle(g_origin, { 15, 15 });	//�^������~
//MOVE_TYPE		g_MoveType = MOVE_TYPE::Constant_Velocity;				//�^�������̎��

// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	Init();

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

	SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//��3����(50ms=1000/220fps)

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
	//char str[256];		//������\���p
	HPEN hPen;			//�y���I�u�W�F�N�g
	HPEN hPenOld;		//�ύX�O�̃I�u�W�F�N�g�ۑ��p
	HBRUSH hBrush;		//�u���V�I�u�W�F�N�g
	HBRUSH hBrushOld;	//�ύX�O�̃I�u�W�F�N�g�ۑ��p


	switch(uMsg)
	{
	case WM_DESTROY:	// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_TIMER:
	{
			switch (wParam) {
			case IDT_TIMER1:

				//���Ԃ̌o��
				g_time += 0.5f;
				g_Viewer.SetTime(g_time);

				//���ԃ��Z�b�g
				POINT pos = g_Circle.GetCircle().GetPos();
				if (pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y > SCREEN_HEIGHT || pos.y < 0) {
					TimeReset();
				}

				g_Circle.Update(g_time);

				//�ĕ`����w��
				RECT rect = {
					0,0, SCREEN_WIDTH, SCREEN_HEIGHT
				};
				InvalidateRect(hWnd, &rect, TRUE);
				break;
			//default:
			//	break;
			}
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
		g_Viewer.Draw(hDC, g_Circle.GetType());

		//�y�����쐬
		hPen = CreatePen(PS_SOLID, 3, GREEN);	//����,����,�F
		//�y���������ւ�
		hPenOld = (HPEN)SelectObject(hDC, hPen);

		//�~��`��
		g_Circle.Draw(hDC);

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
				"�I�����܂���?",
				"�m�F",
				MB_YESNO);
			if (id == IDYES) {
				DestroyWindow(hWnd);
			}
		}
		case VK_UP:
			UpdateMoveType(--const_cast<MOVE_TYPE&>(g_Circle.GetType()));
			TimeReset();
			break;
		case VK_DOWN:
			UpdateMoveType(++const_cast<MOVE_TYPE&>(g_Circle.GetType()));
			TimeReset();
			break;
		case VK_RIGHT:
			if (g_Circle.GetType() == MOVE_TYPE::Oblique_Throw) {
				g_Circle.SetTheta(g_Circle.GetTheta() - 5.0f);
				if (g_Circle.GetTheta() < 0.0f) {
					g_Circle.SetTheta(355.0f);
				}
				g_Viewer.SetTheta(g_Circle.GetTheta());
				TimeReset();
			}
			break;
		case VK_LEFT:
			if (g_Circle.GetType() == MOVE_TYPE::Oblique_Throw) {
				g_Circle.SetTheta(g_Circle.GetTheta() + 5.0f);
				if (g_Circle.GetTheta() > 355.0f) {
					g_Circle.SetTheta(0.0f);
				}
				g_Viewer.SetTheta(g_Circle.GetTheta());
				TimeReset();
			}
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
			TimeReset();
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

void Init(void)
{
	g_time       = 0.0f;

	g_Viewer.SetTime(g_time);
	g_Viewer.SetInitVel(g_Circle.GetInitVel());
	g_Viewer.SetAcc(g_Circle.GetAcc());
	g_Viewer.SetGravity(g_Circle.GetGAcc());
	g_Viewer.SetTheta(g_Circle.GetTheta());
}

void TimeReset(void)
{
	g_time = 0.0f;
}

void UpdateMoveType(MOVE_TYPE type)
{
	g_Circle.ChangeMoveType(type);
	g_Viewer.SetInitVel(g_Circle.GetInitVel());
	g_Viewer.SetAcc(g_Circle.GetAcc());
	g_Viewer.SetGravity(g_Circle.GetGAcc());
	g_Viewer.SetTheta(g_Circle.GetTheta());
}
