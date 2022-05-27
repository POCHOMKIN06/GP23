#include "Window.h"

#include "main.h"

const TCHAR* Oshima::Window::ClassName = TEXT("�Ԃ����Ă͂�����I");

static POINTFLOAT	g_pos[2] = {
	{100.0f, SCREEN_HEIGHT / 2 + 100.0f},	// 1
	{100.0f, SCREEN_HEIGHT / 2 - 100.0f}	// 2
};
static POINTFLOAT	g_size = { 20.0f, 20.0f };	// �T�C�Y
static POINTFLOAT	g_vel[2] = {	// ���x
	{5.0f, -5.0f},
	{5.0f, 5.0f}
};
static float	g_mass[2] = { 40.0f, 60.0f };	// ����
#define BOUNCE_COEFFICIENT	(0.8f)			// ���˕Ԃ�W��
static COLORREF	g_color[2] = {
	RGB(0,255,255),
	RGB(255,0,255)
};

LRESULT CALLBACK Oshima::Window::WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{	
	//�W���I�u�W�F�N�g
	HDC hDC;			//�f�o�C�X�R���e�L�X�g�̃n���h��
	PAINTSTRUCT ps;		//�N���C�A���g�̈�ɕ`�悷�鎞�ɕK�v�ȍ\����
	char str[256];		// ������\���p
	HPEN hPen;			// �y���I�u�W�F�N�g
	HPEN hPenOld;		// �ύX�O�̃y���I�u�W�F�N�g�ۑ��p
	HBRUSH hBrush;		// �u���V�I�u�W�F�N�g
	HBRUSH hBrushOld;	// �ύX�O�̃u���V�I�u�W�F�N�g�ۑ��p

	switch (message_id)
	{
	case WM_TIMER:
	{
		for (int i = 0; i < 2; i++) {
			g_pos[i].x += g_vel[i].x;
			g_pos[i].y += g_vel[i].y;
		}

		// �����蔻��BC
		float fx = g_pos[0].x - g_pos[1].x;
		float fy = g_pos[0].y - g_pos[1].y;
		float flen = fx * fx + fy * fy;
		static bool isHit = false;	// �A���q�b�g�h�~�p
		if (isHit == false && flen <= (g_size.x / 2 + g_size.x / 2) * (g_size.x / 2 + g_size.x / 2)) {
			isHit = true;
			// �Փˌ�̑��x�v�Z
			POINTFLOAT v1New, v2New;
			v1New.x = ((g_mass[0] - BOUNCE_COEFFICIENT * g_mass[1]) * g_vel[0].x + (1.0f + BOUNCE_COEFFICIENT) * g_mass[1] * g_vel[1].x) / (g_mass[0] + g_mass[1]);
			v1New.y = ((g_mass[0] - BOUNCE_COEFFICIENT * g_mass[1]) * g_vel[0].y + (1.0f + BOUNCE_COEFFICIENT) * g_mass[1] * g_vel[1].y) / (g_mass[0] + g_mass[1]);
			v2New.x = ((g_mass[1] - BOUNCE_COEFFICIENT * g_mass[0]) * g_vel[1].x + (1.0f + BOUNCE_COEFFICIENT) * g_mass[0] * g_vel[0].x) / (g_mass[0] + g_mass[1]);
			v2New.y = ((g_mass[1] - BOUNCE_COEFFICIENT * g_mass[0]) * g_vel[1].y + (1.0f + BOUNCE_COEFFICIENT) * g_mass[0] * g_vel[0].y) / (g_mass[0] + g_mass[1]);

			// �i�[
			g_vel[0].x = v1New.x;
			g_vel[0].y = v1New.y;
			g_vel[1].x = v2New.x;
			g_vel[1].y = v2New.y;
		} else {
			isHit = false;
		}

		// �ĕ`����w��
		RECT rect = { 0, 0, SCREEN_WIDTH,SCREEN_HEIGHT };
		InvalidateRect(window_handle, &rect, TRUE);

		break;
	}
	case WM_PAINT:		//�`�施�߂��o��
	{
		//�`��J�n
		hDC = BeginPaint(window_handle, &ps);

		// �e�L�X�g��\��
		wsprintf(str, "SPACE�Ń��Z�b�g");	// �o�b�t�@�Ɋi�[
		//				���W		�@������ƒ���
		TextOut(hDC, 100, 100, str, strlen(str));
		// �e�L�X�g��\��
		wsprintf(str, "���F���̉^���A���F�O�̉^��");	// �o�b�t�@�Ɋi�[
		//				���W		�@������ƒ���
		TextOut(hDC, 100, 120, str, strlen(str));

		//// �e�L�X�g�̐F��ύX
		//SetTextColor(hDC, RGB(255, 0, 0));
		//TextOut(hDC, 300, 200, str, strlen(str));

		// ���S
		MoveToEx(hDC, SCREEN_WIDTH / 2, 0, NULL);		// �n�_
		LineTo(hDC, SCREEN_WIDTH / 2, SCREEN_HEIGHT);	// ���̓_
		MoveToEx(hDC, 0, SCREEN_HEIGHT / 2, NULL);		// �n�_
		LineTo(hDC, SCREEN_WIDTH, SCREEN_HEIGHT / 2);	// �I�_

		for (int i = 0; i < 2; i++) {
			// �y�����쐬
			//							����		�F
			hPen = CreatePen(PS_SOLID, 3, g_color[i]);
			// �y���̎����ւ�
			hPenOld = (HPEN)SelectObject(hDC, hPen);
			// �~��`��
			Ellipse(hDC,
				// ���W+-�T�C�Y�̔���
				(int)(g_pos[i].x - g_size.x / 2),	// ��
				(int)(g_pos[i].y - g_size.y / 2),	// ��
				(int)(g_pos[i].x + g_size.x / 2),	// �E
				(int)(g_pos[i].y + g_size.y / 2));	// ��
			// �y����߂�
			SelectObject(hDC, hPenOld);
			// �쐬�����y���̍폜
			DeleteObject(hPen);
		}

		// �u���V�쐬
		hBrush = CreateSolidBrush(RGB(0, 0, 255));	// �F
		// �u���V�����ւ�
		hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);
		//// �l�p�`��`��
		////				��	��	�E	��
		//Rectangle(hDC, 340, 340, 460, 460);
		// �u���V��߂�
		SelectObject(hDC, hBrushOld);
		// �쐬�����u���V�̍폜
		DeleteObject(hBrush);


		//�`��I��
		EndPaint(window_handle, &ps);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wparam) {
		case VK_SPACE:
			// ���Z�b�g
			break;
		case VK_RIGHT:
			// ���Z�b�g
			break;
		case VK_LEFT:
			// ���Z�b�g
			break;
		case VK_ESCAPE:
			{
				int id = MessageBox(window_handle, "�I�����܂����H", "�m�F", MB_YESNO);
				if (id == IDYES) {
					DestroyWindow(window_handle);
				}
				break;
			}
		}
		break;
	}
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:	// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}

bool Oshima::Window::Create()
{
	if (EntryWindowClass() == FALSE)
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

bool Oshima::Window::EntryWindowClass()
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

void Oshima::Window::ResizeWindow(HWND window_handle)
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
