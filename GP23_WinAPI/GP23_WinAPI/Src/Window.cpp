#include "Window.h"

#include "BegieCurve.h"

const TCHAR* Window::ClassName = TEXT("�T���v��");

namespace{
POINTFLOAT g_pos[4] = {
	{100.0f, 400.0f},	//�n�_
	{200.0f, 100.0f},	//���ԓ_
	{500.0f, 200.0f},	//���ԓ_
	{600.0f, 550.0f}	//�I�_
};
}
BegieCurve BCurve;

LRESULT Window::WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{	
	//�W���I�u�W�F�N�g
	HDC hDC;			//�f�o�C�X�R���e�L�X�g�̃n���h��
	PAINTSTRUCT ps;		//�N���C�A���g�̈�ɕ`�悷�鎞�ɕK�v�ȍ\����

	switch (message_id)
	{
	case WM_CREATE:
	{
		BCurve.SetControlPoint(g_pos[0], g_pos[1], g_pos[2], g_pos[3], FALSE);
		break;
	}
	case WM_PAINT:		//�`�施�߂��o��
	{
		//�`��J�n
		hDC = BeginPaint(window_handle, &ps);

		//
		//�`�揈���͂����ɏ���
		//

		//����_�̕`��
		BCurve.DrawControlPoint(hDC);

		//�Ȑ���`��
		BCurve.Draw(hDC);

		//�`��I��
		EndPaint(window_handle, &ps);
		break;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
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
