#include "Window.h"

#include <time.h>
#include <random>
#include "main.h"
#include "input.h"

#include "Fireworks.h"
#include "NightSky.h"
#include "StarLight.h"
#include "Moon.h"

//#include "BegieCurve.h"
//#include "HermitianCurve.h"
//#include "Laser.h"
//#include "MovableHermitianCurve.h"

const TCHAR* Window::ClassName = TEXT("�T���v��");

//�}�N����`
#define IDT_TIMER1	(100)

namespace
{
	const POINTFLOAT defSPos = { 50, SCREEN_HEIGHT - 100 };
	const POINTFLOAT defEPos = { 50, 100 };
	const POINTFLOAT defSVec = { 0, -500 };
	const POINTFLOAT defEVec = { 0, -500 };


	// �\���̒�`
	typedef struct _tRootData
	{
		POINT	start;	// 
		POINT	end;	// 
		int		t0;		// 
		int		t1;		// 
	}tRootData;

	int g_nTime = 0;
	POINT g_pos = { 0,0 };
#ifdef _DEBUG
	int dif_time = 10;
#else
	int dif_time = 100;
#endif // _DEBUG

	tRootData root[] = {
		{{-50, 250},{150, -50},0,dif_time*static_cast<int>(ColorsName::Color_Num)},

		{-1},
	};
	int	g_nRootIdx = 0;

	Fireworks* Fires[50];
	NightSky Sky;
	StarLight* Stars_s[65];
	StarLight* Stars_m[20];
	StarLight* Stars_l[15];
	Moon* MoveMoon;
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�W���I�u�W�F�N�g
	HDC			hdc;			// �f�o�C�X�R���e�L�X�g�n���h��
	//HBITMAP		hDstBmp;		// �r�b�g�}�b�v�n���h��
	PAINTSTRUCT	ps;				// �N���C�A���g�̈�ɕ`�悷�鎞�ɕK�v�ȍ\����
	static int fire_num = 0;
	//int			i;
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
		//Fireworks
		for (auto&& f : Fires) {
			f = new Fireworks();
			//POINTFLOAT s_pos = { rand() % (SCREEN_WIDTH -50) + defSPos.x, defSPos.y };
			//POINTFLOAT e_pos = { rand() % (SCREEN_WIDTH -50) + defEPos.x, rand() % (SCREEN_HEIGHT / 4) + defEPos.y };
			//POINTFLOAT s_vec = { rand() % (SCREEN_WIDTH / 3) + defSVec.x, rand() % (SCREEN_HEIGHT / 10) + defSVec.y };
			//POINTFLOAT e_vec = { rand() % (SCREEN_WIDTH / 3) + defEVec.x, rand() % (SCREEN_HEIGHT / 10) + defEVec.y };
			//f->Create(30, s_pos, s_vec, e_pos, e_vec);

		}
		//NightSky
		//���ڐ���

		//Starlight
		for (auto&& s : Stars_s) {
			s = new StarLight({ rand() % SCREEN_WIDTH + 0, rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 1 / 4 }, StarLight::SIZE::Small);
		}
		for (auto&& s : Stars_m) {
			s = new StarLight({ rand() % SCREEN_WIDTH + 0, rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 1 / 4 }, StarLight::SIZE::Middle);
		}
		for (auto&& s : Stars_l) {
			s = new StarLight({ rand() % SCREEN_WIDTH + 0, rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 1 / 4 }, StarLight::SIZE::Large);
		}

		//Moon
		MoveMoon = new Moon({ -100, 300 }, 25, 28);
		return 0;
	}
	case WM_TIMER:
	{
		// ���Ԃ̌o��
		g_nTime++;
		// ���`��Ԍv�Z
		FLOAT deltaT =
			(g_nTime - root[g_nRootIdx].t0) /
			(FLOAT)(root[g_nRootIdx].t1 - root[g_nRootIdx].t0);
		g_pos.x =
			root[g_nRootIdx].start.x +
			(LONG)((root[g_nRootIdx].end.x - root[g_nRootIdx].start.x) * deltaT);
		g_pos.y =
			root[g_nRootIdx].start.y +
			(LONG)((root[g_nRootIdx].end.y - root[g_nRootIdx].start.y) * deltaT);

		// ���[�g�̍X�V
		if (g_nTime >= root[g_nRootIdx].t1)	//�I�_�𒴂����玟��Idx��
		{
			g_nRootIdx++;
			if (root[g_nRootIdx].start.x == -1)	// 
			{	// �I�_�Ȃ̂ŏ�����
				g_nRootIdx = 0;
				g_nTime = 0;
			}
		}

		UpdateInput();
		//// ���@�𑀍�
		//if (GetAsyncKeyState(VK_W)) player_y -= MOVE_SPEED;

		//Fireworks����
		using LS = Laser::STATUS;
		//if (g_nTime % 2 == 0) {
			//int fire_create = rand() % 49 + 0;
			//Fires[fire_create]->SetStatus(LS::SHOOT);
		//}

		if (g_nTime % 5 == 0) {
			int fire_create = rand() % 49 + 0;
			Fires[fire_create]->SetStatus(LS::SHOOT);
		}
		for (auto&& f : Fires) {
			f->Update(g_nTime);
		}
		//if (GetKeyTrigger(VK_SPACE) && Fires[fire_num]->GetStatus() == LS::STANDBY) {
		//	Fires[fire_num]->SetStatus(LS::SHOOT);
		//	fire_num++;
		//	if (fire_num >= 50) {
		//		fire_num = 0;
		//	}
		//}
		//FireWorks
		//Fires[fire_create]->Update(g_nTime);
		//NightSky
		Sky.Update(g_nTime);
		//StarLight
		for (auto&& s : Stars_s) {
			s->Update(g_nTime);
		}
		for (auto&& s : Stars_m) {
			s->Update(g_nTime);
		}
		for (auto&& s : Stars_l) {
			s->Update(g_nTime);
		}
		//Moon
		MoveMoon->Update(g_nTime, g_pos);

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
		HBRUSH back_color = CreateSolidBrush(RGB(0.098039225f * 256, 0.098039225f * 256, 0.439215720f * 256)); //MidnightBlue
		FillRect(hdc, &wrect, back_color);
		DeleteObject(back_color);
		
		//Sky
		Sky.Draw(hWnd, hdc);

		//Fireworks
		for (auto&& f : Fires) {
			f->Draw(hdc);
		}
		//Fires[fire_num]->Draw(hdc);

		//Moon
		MoveMoon->Draw(hdc, Sky.GetColor());

		//StarLight
		for (auto&& s : Stars_s) {
			s->Draw(hdc);
		}
		for (auto&& s : Stars_m) {
			s->Draw(hdc);
		}
		for (auto&& s : Stars_l) {
			s->Draw(hdc);
		}

		//�n��
		HPEN gpen = CreatePen(PS_SOLID, 1, Sky.GetColor());
		//HBRUSH ground = CreateSolidBrush(RGB(100, 15, 35));
		HBRUSH groundh = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 15, 35));
		HPEN oldP = (HPEN)SelectObject(hdc, gpen);
		//HBRUSH oldG = (HBRUSH)SelectObject(hdc, ground);
		HBRUSH oldG = (HBRUSH)SelectObject(hdc, groundh);
		::Rectangle(hdc, -5, SCREEN_HEIGHT - 100, SCREEN_WIDTH+5, SCREEN_HEIGHT+5);
		DeleteObject(SelectObject(hdc, oldP));
		DeleteObject(SelectObject(hdc, oldG));


		//COLORREF DefaultC = SetTextColor(hdc, (COLORREF)GetStockObject(WHITE_PEN));
		//SetBkMode(hdc, TRANSPARENT);
		//TEXTMETRIC tm;
		//GetTextMetrics(hdc, &tm);

		//// ����_
		//wsprintf(str, TEXT("WASD : �v���C���["));
		//TextOut(hdc, 5, 5 + tm.tmHeight * 0, str, _tcslen(str));
		//wsprintf(str, TEXT("SPACE : ����"));
		//TextOut(hdc, 5, 5 + tm.tmHeight * 1, str, _tcslen(str));
		//wsprintf(str, TEXT("�������� : �G"));
		//TextOut(hdc, 200, 5 + tm.tmHeight * 0, str, _tcslen(str));
		//SetBkMode(hdc, OPAQUE);
		//SetTextColor(hdc, DefaultC);
#ifdef _DEBUG
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 5, 5, Sky.GetColorName(), _tcslen(Sky.GetColorName()));
		wsprintf(str, TEXT("g_time : %d"), g_nTime);
		TextOut(hdc, 150, 5, str, _tcslen(str));
		SetBkMode(hdc, OPAQUE);

#endif // _DEBUG

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
		for (auto&& f : Fires) {
			delete f;
		}
		for (auto&& s : Stars_s) {
			delete s;
		}
		for (auto&& s : Stars_m) {
			delete s;
		}
		for (auto&& s : Stars_l) {
			delete s;
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
