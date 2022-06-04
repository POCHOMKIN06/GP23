#include "Window.h"

#include "main.h"
#include "BegieCurve.h"

const TCHAR* Window::ClassName = TEXT("�T���v��");

//�}�N����`
#define IDT_TIMER1	(100)

POINTFLOAT g_pos[4] = {
	{100.0f, 400.0f},	//�n�_
	{200.0f, 100.0f},	//���ԓ_
	{500.0f, 200.0f},	//���ԓ_
	{600.0f, 550.0f}	//�I�_
};


enum _bmp {
	BMP_BG = 0,		// 0
	BMP_PLAYER,		// 1
	BMP_ENEMY,		// 2
	BMP_QTY,		// 3 BMP�̑���������
};

LPCTSTR		bmp_file[] = {
	_T("Bmp/bg.bmp"),		// 0 �a�f
	_T("Bmp/player.bmp"),	// 1 ���@
	_T("Bmp/enemy.bmp"),	// 2 �G�@
};

const UINT	MOVE_SPEED = 8;				// ���@�ړ��X�s�[�h
const UINT	BG_W = 640;				// �a�f�@��
const UINT	BG_H = 480;				// �a�f�@����
const UINT	PLAYER_W = 48;				// ���@�@��
const UINT	PLAYER_H = 32;				// ���@�@����
const UINT	ENEMY_W = 32;				// �G�@�@��
const UINT	ENEMY_H = 32;				// �G�@�@����
const UINT	CURVE_DIV = 10;				// �Ȑ�������

const float	PLAYER_VECTOR_X = -500.0f;		// ���@���@�ڐ��x�N�g���@�w����
const float	PLAYER_VECTOR_Y = -500.0f;		// ���@���@�ڐ��x�N�g���@�x����
const float	ENEMY_VECTOR_X = 300.0f;		// �G�@���@�ڐ��x�N�g���@�w����
const float	ENEMY_VECTOR_Y = 200.0f;		// �G�@���@�ڐ��x�N�g���@�x����

//-------- �\���̒�`
struct LASER {
	int		status;							// �X�e�[�^�X
	int		start;							// ���[�U�[�J�n�C���f�b�N�X
	int		end;							// ���[�U�[�I���C���f�b�N�X
	POINTFLOAT	curve_pt[CURVE_DIV + 1];	// �G���~�[�g�Ȑ���̍��W
};

BegieCurve BCurve;

HDC			g_hMemDC[BMP_QTY];				// �������c�b�i�a�l�o�ێ��p�j
int			player_x, player_y;				// ���@���W
int			enemy_x, enemy_y;				// �G�@���W

LASER		laser;							// ���[�U�[



LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�W���I�u�W�F�N�g
	HDC			hdc;			// �f�o�C�X�R���e�L�X�g�n���h��
	HBITMAP		hDstBmp;		// �r�b�g�}�b�v�n���h��
	PAINTSTRUCT	ps;				//�N���C�A���g�̈�ɕ`�悷�鎞�ɕK�v�ȍ\����
	HPEN		hOldPen;
	int			i;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		BCurve.SetControlPoint(g_pos[0], g_pos[1], g_pos[2], g_pos[3], FALSE);

		// �r�b�g�}�b�v�t�@�C�������[�h
		hdc = GetDC(hWnd);
		for (i = 0; i < BMP_QTY; i++) {
			hDstBmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), bmp_file[i], IMAGE_BITMAP, 0, 0,
				LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			g_hMemDC[i] = CreateCompatibleDC(hdc);				// �������c�b���쐬
			SelectObject(g_hMemDC[i], hDstBmp);					// �r�b�g�}�b�v��I��
			DeleteObject(hDstBmp);								// ���[�h�����r�b�g�}�b�v��j��
		}
		ReleaseDC(hWnd, hdc);

		// �ϐ�������
		player_x = 120;
		player_y = 240;
		enemy_x = 550;
		enemy_y = 240;
		laser.status = 0;

		SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//��3����:50ms(1000/20fps)

		return 0;
	}
	case WM_TIMER:
	{
		// ���@�𑀍�
		if (GetAsyncKeyState(VK_UP)) player_y -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_DOWN)) player_y += MOVE_SPEED;
		if (GetAsyncKeyState(VK_LEFT)) player_x -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_RIGHT)) player_x += MOVE_SPEED;
		if (GetAsyncKeyState(VK_SPACE) && laser.status == 0) laser.status = 1;	// ���[�U�[�𔭎�
		// �ĕ`���v��
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_ESCAPE:
			KillTimer(hWnd, IDT_TIMER1);
			int ret = MessageBox(hWnd, _T("�I�����܂���?"), _T("�I���m�F"), MB_OKCANCEL);
			if (ret == IDOK) {
				DestroyWindow(hWnd);	// �E�B���h�E�j�����w��
			} else {
				SetTimer(hWnd, IDT_TIMER1, 1000 / FPS, NULL);
			}
			return 0;
		}
		break;
	}
	case WM_PAINT:		//�`�施�߂��o��
	{
		//�`��J�n
		hdc = BeginPaint(hWnd, &ps);

		//------------------------------------------
		// �x�W�G�Ȑ��̕`��
		//------------------------------------------
		// ����_�̕`��
		BCurve.DrawControlPoint(hdc);
		// �Ȑ���`��
		BCurve.Draw(hdc);


		BitBlt(hdc, 0, 0, BG_W, BG_H, g_hMemDC[BMP_BG], 0, 0, SRCCOPY);	// �a�f�`��
		BitBlt(hdc, player_x, player_y, PLAYER_W, PLAYER_H, g_hMemDC[BMP_PLAYER], 0, 0, SRCCOPY);	// ���@�`��
		BitBlt(hdc, enemy_x, enemy_y, ENEMY_W, ENEMY_H, g_hMemDC[BMP_ENEMY], 0, 0, SRCCOPY);	// �G�@�`��

		// ���[�U�[��`��
		hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(0, 255, 255)));	// ���[�U�[�p�y�����쐬


		//----------------------------------------------------------------
		//  �����ɃG���~�[�g�Ȑ����g��������������
		//  �Ȑ����[�U�[��`�悳����
		//----------------------------------------------------------------
		float h00, h01, h10, h11;
		float t;

		//�G���~�[�g�Ȑ����v�Z
		for (i = 0; i <= CURVE_DIV; i++) {
			t = (float)i / CURVE_DIV;
			h00 = (t - 1)*(t - 1)*(2 * t + 1);		// �G���~�[�g��Ԋ֐�H0,0
			h01 = t * t*(3 - 2 * t);				// �G���~�[�g��Ԋ֐�H0,1
			h10 = (1 - t * t)*t;					// �G���~�[�g��Ԋ֐�H1,0
			h11 = (t - 1)*t*t;						// �G���~�[�g��Ԋ֐�H1,1
			laser.curve_pt[i].x = h00 * player_x + h01 * enemy_x + h10 * PLAYER_VECTOR_X + h11 * ENEMY_VECTOR_X;
			laser.curve_pt[i].y = h00 * player_y + h01 * enemy_y + h10 * PLAYER_VECTOR_Y + h11 * ENEMY_VECTOR_Y;
		}
		//�`��p�n�_�ƏI�_��ݒ�
		switch (laser.status)
		{
		case 0:
			laser.start = 0;
			laser.end = 0;
			break;
		case 1:
			if (laser.end < CURVE_DIV) {	// ���[�U�[��G�܂ŐL�΂�
				laser.end++;
			} else {	// ����
				laser.status = 2;
			}
			break;
			//laser.start = 0;
			//laser.end = CURVE_DIV;
			//break;
		default:
			break;
		}

		//�G���~�[�g�Ȑ���`��
		MoveToEx(hdc, static_cast<int>(laser.curve_pt[laser.start].x), static_cast<int>(laser.curve_pt[laser.start].y), NULL);
		for (i = laser.start + 1; i <= laser.end; i++) {
			LineTo(hdc, static_cast<int>(laser.curve_pt[i].x), static_cast<int>(laser.curve_pt[i].y));
		}

		//�`��I��
		DeleteObject(SelectObject(hdc, hOldPen));	// �쐬�����y�����폜
		EndPaint(hWnd, &ps);
		break;
		
	}
	case WM_CLOSE:
	{
		for (i = 0; i < BMP_QTY; i++) {
			DeleteDC(g_hMemDC[i]);				// �������c�b���폜
		}
		DestroyWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:							// �I���w��������
	{
		KillTimer(hWnd, IDT_TIMER1);			// �^�C�}�[���~
		PostQuitMessage(0);						// �V�X�e���ɃX���b�h�̏I����v��
		return 0;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Window::Create()
{
	if (EntryWindowClass() == false)
	{
		return false;
	}

	// �E�B���h�E�쐬
	WindowHandle_ = CreateWindow(
		Window::ClassName,
		Title_,
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE,
		CW_USEDEFAULT,
		0,
		Width_,
		Height_,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if (WindowHandle_ == NULL)
	{
		return false;
	}

	ResizeWindow(WindowHandle_);

	return true;
}

bool Window::EntryWindowClass()
{
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX),				// �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,		// �N���X�̃X�^�C��
		WindowProc,						// �E�B���h�E�v���V�[�W��
		0,								// �⏕������
		0,								// �⏕������
		GetModuleHandle(NULL),			// ���̃v���O�����̃C���X�^���X�n���h��
		LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)),// �A�C�R���摜
		LoadCursor(NULL, IDC_ARROW),	// �J�[�\���摜
		NULL,							// �w�i�u���V(�w�i�F)
		NULL,							// ���j���[��
		Window::ClassName,				// �N���X��									
		NULL							// �������A�C�R��
	};

	// �\���̂̓o�^
	if (RegisterClassEx(&window_class) == 0)
	{
		return false;
	}

	return true;
}

void Window::ResizeWindow(HWND window_handle)
{
	RECT window_rect;
	RECT client_rect;

	GetWindowRect(window_handle, &window_rect);
	GetClientRect(window_handle, &client_rect);

	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	SetWindowPos(window_handle, NULL, CW_USEDEFAULT, 0, frame_size_x + Width_, frame_size_y + Height_, SWP_NOMOVE);

	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);
}
