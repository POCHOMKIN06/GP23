#include "Window2.h"

#include "main.h"
#include "input.h"

#include "MovableHermitianCurve.h"

const TCHAR* Window2::ClassName = TEXT("�T���v��2");

//�}�N����`
//#define IDT_TIMER1	(100)
namespace
{
	//POINTFLOAT g_pos[4] = {
	//	{100.0f, 400.0f},	//�n�_
	//	{200.0f, 100.0f},	//���ԓ_
	//	{500.0f, 200.0f},	//���ԓ_
	//	{600.0f, 550.0f}	//�I�_
	//};
	//

	//enum _bmp {
	//	BMP_BG = 0,		// 0
	//	BMP_PLAYER,		// 1
	//	BMP_ENEMY,		// 2
	//	BMP_QTY,		// 3 BMP�̑���������
	//};
	//
	//LPCTSTR		bmp_file[] = {
	//	_T("Bmp/bg.bmp"),		// 0 �a�f
	//	_T("Bmp/player.bmp"),	// 1 ���@
	//	_T("Bmp/enemy.bmp"),	// 2 �G�@
	//};
	//
	//const UINT	MOVE_SPEED = 8;				// ���@�ړ��X�s�[�h
	//const UINT	BG_W = 640;					// �a�f�@��
	//const UINT	BG_H = 480;					// �a�f�@����
	//const UINT	PLAYER_W = 48;				// ���@�@��
	//const UINT	PLAYER_H = 32;				// ���@�@����
	//const UINT	ENEMY_W = 32;				// �G�@�@��
	//const UINT	ENEMY_H = 32;				// �G�@�@����
	//const UINT	CURVE_DIV = 10;				// �Ȑ�������
	//
	//const float	PLAYER_VECTOR_X = -500.0f;		// ���@���@�ڐ��x�N�g���@�w����
	//const float	PLAYER_VECTOR_Y = -500.0f;		// ���@���@�ڐ��x�N�g���@�x����
	//const float	ENEMY_VECTOR_X = 300.0f;		// �G�@���@�ڐ��x�N�g���@�w����
	//const float	ENEMY_VECTOR_Y = 200.0f;		// �G�@���@�ڐ��x�N�g���@�x����

	//-------- �\���̒�`
	//struct LASER {
	//	int		status;							// �X�e�[�^�X
	//	int		start;							// ���[�U�[�J�n�C���f�b�N�X
	//	int		end;							// ���[�U�[�I���C���f�b�N�X
	//	POINTFLOAT	curve_pt[CURVE_DIV + 1];	// �G���~�[�g�Ȑ���̍��W
	//};

	//BegieCurve BCurve;

	//HDC			g_hMemDC[BMP_QTY];				// �������c�b�i�a�l�o�ێ��p�j
	//int			player_x, player_y;				// ���@���W
	//int			enemy_x, enemy_y;				// �G�@���W

	//LASER		laser;							// ���[�U�[

	//Laser laser;
	static MovableHermitianCurve MHCurve;

	//int at;
}
LRESULT Window2::WindowProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�W���I�u�W�F�N�g
	HDC			hdc;			// �f�o�C�X�R���e�L�X�g�n���h��
	HBITMAP		hDstBmp;		// �r�b�g�}�b�v�n���h��
	PAINTSTRUCT	ps;				//�N���C�A���g�̈�ɕ`�悷�鎞�ɕK�v�ȍ\����
	int			i;
	static unsigned short int mouse_x = -10, mouse_y = 10;
	TCHAR str[256];
	static int check = -1;
	static bool check1[4];

	switch (uMsg)
	{
	case WM_CREATE:
	{
		//BCurve.SetControlPoint(g_pos[0], g_pos[1], g_pos[2], g_pos[3], FALSE);

		// �r�b�g�}�b�v�t�@�C�������[�h
		//hdc = GetDC(hWnd);
		//for (i = 0; i < BMP_QTY; i++) {
		//	hDstBmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), bmp_file[i], IMAGE_BITMAP, 0, 0,
		//		LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		//	g_hMemDC[i] = CreateCompatibleDC(hdc);				// �������c�b���쐬
		//	SelectObject(g_hMemDC[i], hDstBmp);					// �r�b�g�}�b�v��I��
		//	DeleteObject(hDstBmp);								// ���[�h�����r�b�g�}�b�v��j��
		//}
		//ReleaseDC(hWnd, hdc);

		//// �ϐ�������
		//player_x = 120;
		//player_y = 240;
		//enemy_x = 550;
		//enemy_y = 240;

		//at = 270;

		//if (laser.Create(10,
		//	POINTFLOAT{ (float)player_x, (float)player_y }, POINTFLOAT{ PLAYER_VECTOR_X, PLAYER_VECTOR_Y },
		//	POINTFLOAT{ (float)enemy_x,  (float)enemy_y }, POINTFLOAT{ ENEMY_VECTOR_X, ENEMY_VECTOR_Y })
		//	== FALSE) {
		//	MessageBox(hWnd, TEXT("laser�̐����Ɏ��s"), TEXT("�����G���["), MB_OK);
		//	PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		//	break;
		//}

		if (MHCurve.Create(20,
			POINTFLOAT{ 100.0f, 340.0f }, POINTFLOAT{ 200.0f, 100.0f },
			POINTFLOAT{ 320.0f,  80.0f }, POINTFLOAT{ 200.0f, 100.0f })
			== FALSE) {
			MessageBox(hWnd, TEXT("MHCurve�̐����Ɏ��s"), TEXT("�����G���["), MB_OK);
			PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}

		//SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//��3����:50ms(1000/20fps)

		return 0;
	}
	case WM_TIMER:
	{
		//// ���@�𑀍�
		//if (GetAsyncKeyState(VK_W)) player_y -= MOVE_SPEED;
		//if (GetAsyncKeyState(VK_S)) player_y += MOVE_SPEED;
		//if (GetAsyncKeyState(VK_A)) player_x -= MOVE_SPEED;
		//if (GetAsyncKeyState(VK_D)) player_x += MOVE_SPEED;
		//// �G�@�𑀍�
		//if (GetAsyncKeyState(VK_UP))	enemy_y -= MOVE_SPEED;
		//if (GetAsyncKeyState(VK_DOWN))	enemy_y += MOVE_SPEED;
		//if (GetAsyncKeyState(VK_LEFT))	enemy_x -= MOVE_SPEED;
		//if (GetAsyncKeyState(VK_RIGHT))	enemy_x += MOVE_SPEED;

		//laser.Update(POINTFLOAT{ (float)player_x, (float)player_y }, POINTFLOAT{ (float)enemy_x, (float)enemy_y });

		//using LS = Laser::STATUS;
		//if (GetAsyncKeyState(VK_SPACE) && laser.GetStatus() == LS::STANDBY) laser.SetStatus(LS::SHOOT);	// ���[�U�[�𔭎�

		//at -= 5;
		//if (at <= 0) {
		//	at = 270;
		//}
		// �ĕ`���v��
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if (wParam & MK_LBUTTON) {

			mouse_x = LOWORD(lParam);
			mouse_y = HIWORD(lParam);

			RECT rect = MHCurve.GetRect();

			MovableHermitianCurve::ContPos cp = MHCurve.GetControlPos();
			RECT c_rect[4];
			c_rect[0] = RECT{ (int)(cp.s_pos.x + rect.left),	 (int)(cp.s_pos.y + rect.top),	   (int)(cp.s_pos.x + rect.right),	   (int)(cp.s_pos.y + rect.bottom) };
			c_rect[1] = RECT{ (int)(cp.s_vec_pos.x + rect.left), (int)(cp.s_vec_pos.y + rect.top), (int)(cp.s_vec_pos.x + rect.right), (int)(cp.s_vec_pos.y + rect.bottom) };
			c_rect[2] = RECT{ (int)(cp.e_pos.x + rect.left),	 (int)(cp.e_pos.y + rect.top),	   (int)(cp.e_pos.x + rect.right),	   (int)(cp.e_pos.y + rect.bottom) };
			c_rect[3] = RECT{ (int)(cp.e_vec_pos.x + rect.left), (int)(cp.e_vec_pos.y + rect.top), (int)(cp.e_vec_pos.x + rect.right), (int)(cp.e_vec_pos.y + rect.bottom) };


			if (mouse_x >= c_rect[0].left && mouse_x <= c_rect[0].right
				&& mouse_y >= c_rect[0].top && mouse_y <= c_rect[0].bottom) {
				check1[0] = true;
			}
			if (mouse_x >= c_rect[1].left && mouse_x <= c_rect[1].right
				&& mouse_y >= c_rect[1].top && mouse_y <= c_rect[1].bottom) {
				check1[1] = true;
			}
			if (mouse_x >= c_rect[2].left && mouse_x <= c_rect[2].right
				&& mouse_y >= c_rect[2].top && mouse_y <= c_rect[2].bottom) {
				check1[2] = true;
			}
			if (mouse_x >= c_rect[3].left && mouse_x <= c_rect[3].right
				&& mouse_y >= c_rect[3].top && mouse_y <= c_rect[3].bottom) {
				check1[3] = true;
			}

			if (check1[0] == true) {
				MHCurve.MovePos(POINTFLOAT{ (float)mouse_x, (float)mouse_y }, 0);
			}
			if (check1[1] == true) {
				MHCurve.MovePos(POINTFLOAT{ (float)mouse_x, (float)mouse_y }, 1);
			}
			if (check1[2] == true) {
				MHCurve.MovePos(POINTFLOAT{ (float)mouse_x, (float)mouse_y }, 2);
			}
			if (check1[3] == true) {
				MHCurve.MovePos(POINTFLOAT{ (float)mouse_x, (float)mouse_y }, 3);
			}

			//if (MHCurve.CheckMousePos(mouse_x, mouse_y, &check)) {
			//	MHCurve.MovePos(POINTFLOAT{ (float)mouse_x, (float)mouse_y }, check);
			//}

			RECT wrect;
			GetClientRect(hWnd, &wrect);
			InvalidateRect(hWnd, &wrect, TRUE);
			}
		break;
	}
	case WM_LBUTTONUP:
	{
			for (int i = 0; i < 4; i++) {
				check1[i] = false;
			}
			check = -1;
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_ESCAPE:
			//KillTimer(hWnd, IDT_TIMER1);
			int ret = MessageBox(hWnd, _T("�I�����܂���?"), _T("�I���m�F"), MB_OKCANCEL);
			if (ret == IDOK) {
				DestroyWindow(hWnd);	// �E�B���h�E�j�����w��
			} else {
				//SetTimer(hWnd, IDT_TIMER1, 1000 / FPS, NULL);
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
		//// ����_�̕`��
		//BCurve.DrawControlPoint(hdc);
		//// �Ȑ���`��
		//BCurve.Draw(hdc);


		//BitBlt(hdc, 0, 0, BG_W, BG_H, g_hMemDC[BMP_BG], 0, 0, SRCCOPY);	// �a�f�`��
		//BitBlt(hdc, player_x, player_y, PLAYER_W, PLAYER_H, g_hMemDC[BMP_PLAYER], 0, 0, SRCCOPY);	// ���@�`��
		//BitBlt(hdc, enemy_x, enemy_y, ENEMY_W, ENEMY_H, g_hMemDC[BMP_ENEMY], 0, 0, SRCCOPY);	// �G�@�`��
		
#ifdef _DEBUG
		_stprintf_s(str, TEXT("%d, %d"), mouse_x, mouse_y);
		TextOut(hdc, 10, 250, str, _tcslen(str));
#endif // _DEBUG

		// ���[�U�[��`��

		//----------------------------------------------------------------
		//  �����ɃG���~�[�g�Ȑ����g��������������
		//  �Ȑ����[�U�[��`�悳����
		//----------------------------------------------------------------
		//laser.Draw(hdc);

		MHCurve.Draw(hdc);
		//HPEN old = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0)));
		//MoveToEx(hdc, 100+50 * cos(45), 100-50 * sin(45), NULL);
		//Rectangle(hdc, 210, 10, 400, 200);
		//HBRUSH oldb = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(0, 150, 150)));
		//AngleArc(hdc, 100, 100, 50, 45, at);
		//Pie(hdc, 210, 10, 400, 200, 0, 100, at, 0);
		//DeleteObject(SelectObject(hdc, old));
		//DeleteObject(SelectObject(hdc, oldb));
		
		//�`��I��
		EndPaint(hWnd, &ps);
		break;
		
	}
	case WM_CLOSE:
	{
		//for (i = 0; i < BMP_QTY; i++) {
		//	DeleteDC(g_hMemDC[i]);				// �������c�b���폜
		//}
		DestroyWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:							// �I���w��������
	{
		//KillTimer(hWnd, IDT_TIMER1);			// �^�C�}�[���~
		PostQuitMessage(0);						// �V�X�e���ɃX���b�h�̏I����v��
		return 0;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Window2::Create()
{
	if (EntryWindowClass() == false)
	{
		return false;
	}

	// �E�B���h�E�쐬
	WindowHandle_ = CreateWindow(
		Window2::ClassName,
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

bool Window2::EntryWindowClass()
{
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX),				// �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,		// �N���X�̃X�^�C��
		WindowProc2,						// �E�B���h�E�v���V�[�W��
		0,								// �⏕������
		0,								// �⏕������
		GetModuleHandle(NULL),			// ���̃v���O�����̃C���X�^���X�n���h��
		LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)),// �A�C�R���摜
		LoadCursor(NULL, IDC_ARROW),	// �J�[�\���摜
		NULL,							// �w�i�u���V(�w�i�F)
		NULL,							// ���j���[��
		Window2::ClassName,				// �N���X��									
		NULL							// �������A�C�R��
	};

	// �\���̂̓o�^
	if (RegisterClassEx(&window_class) == 0)
	{
		return false;
	}

	return true;
}

void Window2::ResizeWindow(HWND window_handle)
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
