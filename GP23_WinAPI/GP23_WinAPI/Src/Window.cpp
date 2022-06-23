#include "Window.h"

#include <time.h>
#include <random>
#include "main.h"
#include "input.h"

#include "Fireworks.h"
#include "NightSky.h"
#include "StarLight.h"

//#include "BegieCurve.h"
//#include "HermitianCurve.h"
#include "Laser.h"
//#include "MovableHermitianCurve.h"

const TCHAR* Window::ClassName = TEXT("�T���v��");

//�}�N����`
#define IDT_TIMER1	(100)

namespace
{

	Fireworks* Fires[50];
	NightSky Sky;
	StarLight* Stars[100];
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�W���I�u�W�F�N�g
	HDC			hdc;			// �f�o�C�X�R���e�L�X�g�n���h��
	HBITMAP		hDstBmp;		// �r�b�g�}�b�v�n���h��
	PAINTSTRUCT	ps;				// �N���C�A���g�̈�ɕ`�悷�鎞�ɕK�v�ȍ\����
	int fire_num = 0;
	int			i;
	static unsigned short int mouse_x = -10, mouse_y = 10;
	_TCHAR str[256];


	switch (uMsg)
	{
	case WM_CREATE:
	{
		// �ϐ�������
		srand((unsigned int)time(NULL));
		SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//��3����:50ms(1000/20fps)
		InitInput();

		//�C���X�^���X�̐���
		for (auto&& f : Fires) {
			f = new Fireworks();
		}
		for (auto&& s : Stars) {
			s = new StarLight(
				POINT{ rand() % SCREEN_WIDTH + SCREEN_WIDTH / 2, rand() % SCREEN_HEIGHT + SCREEN_HEIGHT / 2 },
				StarLight::SIZE::Small);
		}

		return 0;
	}
	case WM_TIMER:
	{
		UpdateInput();
		//// ���@�𑀍�
		//if (GetAsyncKeyState(VK_W)) player_y -= MOVE_SPEED;

		using LS = Laser::STATUS;
		if (GetKeyTrigger(VK_SPACE) && Fires[fire_num]->GetStatus() == LS::STANDBY) {
			Fires[fire_num]->SetStatus(LS::SHOOT);
			fire_num++;
			if (fire_num >= 50) {
				fire_num = 0;
			}
		}
		//FireWorkd
		Fires[fire_num]->Update();
		//NightSky
		Sky.Update();
		//StarLight
		for (auto&& s : Stars) {
			s->Update();
		}

		// �ĕ`���v��
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		break;
	}
	case WM_LBUTTONUP:
	{
		break;
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
		// �`��O�ɉ�ʃN���A
		RECT wrect;
		GetClientRect(hWnd, &wrect);
		HBRUSH back_color = CreateSolidBrush(RGB(0.098039225f*256, 0.098039225f*256, 0.439215720f*256)); //MidnightBlue
		FillRect(hdc, &wrect, back_color);
		DeleteObject(back_color);
		
		//Sky
		//Sky.Draw(hWnd, hdc);

		//Fireworks
		Fires[fire_num]->Draw(hdc);

		//StarLight
		for (auto&& s : Stars) {
			s->Draw(hdc);
		}
		

		COLORREF DefaultC = SetTextColor(hdc, (COLORREF)GetStockObject(WHITE_PEN));
		SetBkMode(hdc, TRANSPARENT);
		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);

		// ����_
		wsprintf(str, TEXT("WASD : �v���C���["));
		TextOut(hdc, 5, 5 + tm.tmHeight * 0, str, _tcslen(str));
		wsprintf(str, TEXT("SPACE : ����"));
		TextOut(hdc, 5, 5 + tm.tmHeight * 1, str, _tcslen(str));
		wsprintf(str, TEXT("�������� : �G"));
		TextOut(hdc, 200, 5 + tm.tmHeight * 0, str, _tcslen(str));
		SetBkMode(hdc, OPAQUE);
		SetTextColor(hdc, DefaultC);

		//�`��I��
		EndPaint(hWnd, &ps);
		break;
		
	}
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:							// �I���w��������
	{
		UninitInput();
		KillTimer(hWnd, IDT_TIMER1);			// �^�C�}�[���~
		//�C���X�^���X�̍폜
		for (int i = 0; i < 50; i++) {
			delete Fires[i];
		}
		for (int i = 0; i < 100; i++) {
			delete Stars[i];
		}

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
