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
#define IDT_TIMER1	(100)

// �v���g�^�C�v�錾
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(void);
void TimeReset(void);
void UpdateMoveType(void);

//�p�����[�^�v���Z�b�g
constexpr float INIT_VEL     = 10.0f;
constexpr float ACCELERATION = 4.0f;
constexpr float GRAVITY      = -9.8f;

// �O���[�o���ϐ�
HINSTANCE g_hInstance;
float g_time;
float g_initvel;
float g_acc;
float g_gravityAcc;
float g_theta;
POINT g_origin = { 200, SCREEN_HEIGHT / 2 };

//����I�u�W�F�N�g
AxisLine		g_AxisLine(g_origin.x, g_origin.y);		//���W��
Viewer			g_Viewer;								//�e��p�����[�^�\��
MOVE_TYPE		g_MoveType = MOVE_TYPE::Constant_Velocity;				//�^�������̎��
MovingCircle	g_Circle[static_cast<unsigned int>(MOVE_TYPE::END)];	//�^������~



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
		switch (wParam)
		{
		case IDT_TIMER1:
			//�f�[�^�R�s�[
			MovingCircle* temp = &g_Circle[static_cast<unsigned int>(g_MoveType)];

			//���Ԃ̌o��
			g_time += 0.5f;
			g_Viewer.SetTime(g_time);

			//���ԃ��Z�b�g
			POINT pos = temp->GetPos();
			if (pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y > SCREEN_HEIGHT || pos.y < 0) {
				TimeReset();
			}

			switch (g_MoveType)
			{
			case MOVE_TYPE::Constant_Velocity://�����^��
				pos.x = static_cast<int>(g_origin.x + g_initvel * g_time);
				pos.y = static_cast<int>(g_origin.y);
				break;
			case MOVE_TYPE::Constant_Acceleration://�������^��
				pos.x = static_cast<int>(g_origin.x + g_initvel * g_time + 0.5f*g_acc*g_time*g_time);
				pos.y = static_cast<int>(g_origin.y);
				break;
			case MOVE_TYPE::Free_Fall://���R�����^��
				pos.x = static_cast<int>(g_origin.x);
				pos.y = static_cast<int>(g_origin.y - 0.5f*g_gravityAcc*g_time*g_time);
				break;
			case MOVE_TYPE::Vertical_Throw_Up://���������グ�^��
				pos.x = static_cast<int>(g_origin.x);
				pos.y = static_cast<int>(g_origin.y - g_initvel * g_time - 0.5f*g_gravityAcc*g_time*g_time);
				break;
			case MOVE_TYPE::Oblique_Throw://�Ε����ˉ^��
				pos.x = static_cast<int>(g_origin.x - g_initvel * g_time * cos((-g_theta + 180.0f)*M_PI / 180.0f));
				pos.y = static_cast<int>(g_origin.y - g_initvel * g_time * sin(g_theta*M_PI / 180.0f) - 0.5f*g_gravityAcc*g_time*g_time);
				break;
			case MOVE_TYPE::END:
				break;
			default:
				break;
			}
			temp->SetPos(pos);


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
		g_Viewer.Draw(hDC, g_MoveType);

		//�y�����쐬
		hPen = CreatePen(PS_SOLID, 3, GREEN);	//����,����,�F
		//�y���������ւ�
		hPenOld = (HPEN)SelectObject(hDC, hPen);

		//�~��`��
		g_Circle[static_cast<unsigned int>(g_MoveType)].Draw(hDC);

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
			--g_MoveType;
			TimeReset();
			UpdateMoveType();
			break;
		case VK_DOWN:
			++g_MoveType;
			TimeReset();
			UpdateMoveType();
			break;
		case VK_RIGHT:
			if (g_MoveType == MOVE_TYPE::Oblique_Throw) {
				g_theta -= 5.0f;
				if (g_theta < 0.0f) {
					g_theta = 355.0f;
				}
				g_Viewer.SetTheta(g_theta);
				TimeReset();
			}
			break;
		case VK_LEFT:
			if (g_MoveType == MOVE_TYPE::Oblique_Throw) {
				g_theta += 5.0f;
				if (g_theta > 355.0f) {
					g_theta = 0.0;
				}
				g_Viewer.SetTheta(g_theta);
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
	for (auto&& c : g_Circle) {
		c.SetPos(g_origin);
		c.SetSize({ 15, 15 });
	}

	g_time       = 0.0f;
	g_initvel    = INIT_VEL;
	g_acc        = 0.0f;
	g_gravityAcc = 0.0f;
	g_theta      = 30.0f;

	g_Viewer.SetTime(g_time);
	g_Viewer.SetInitVel(g_initvel);
	g_Viewer.SetAcc(g_acc);
	g_Viewer.SetGravity(g_gravityAcc);
}

void TimeReset(void)
{
	g_time = 0.0f;
}

void UpdateMoveType(void)
{
	switch (g_MoveType) {
	case MOVE_TYPE::Constant_Velocity:
		g_initvel = INIT_VEL;
		g_acc = 0.0f;
		g_gravityAcc = 0.0f;
		break;
	case MOVE_TYPE::Constant_Acceleration:
		g_initvel = INIT_VEL;
		g_acc = ACCELERATION;
		g_gravityAcc = 0.0f;
		break;
	case MOVE_TYPE::Free_Fall:
		g_initvel = 0.0f;
		g_acc = 0.0f;
		g_gravityAcc = GRAVITY;
		break;
	case MOVE_TYPE::Vertical_Throw_Up:
		g_initvel = 70.0f;
		g_acc = 0.0f;
		g_gravityAcc = GRAVITY;
		break;
	case MOVE_TYPE::Oblique_Throw:
		g_initvel = 80.0f;
		g_acc = 0.0f;
		g_gravityAcc = GRAVITY;
		break;
	case MOVE_TYPE::END:
		break;
	default:
		break;
	}
	g_Viewer.SetInitVel(g_initvel);
	g_Viewer.SetAcc(g_acc);
	g_Viewer.SetGravity(g_gravityAcc);
	g_Viewer.SetTheta(g_theta);
}
