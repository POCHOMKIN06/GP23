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

const TCHAR* Window::ClassName = TEXT("サンプル");

//マクロ定義
#define IDT_TIMER1	(100)

namespace
{
	const POINTFLOAT defSPos = { 50, SCREEN_HEIGHT - 100 };
	const POINTFLOAT defEPos = { 50, 100 };
	const POINTFLOAT defSVec = { 0, -500 };
	const POINTFLOAT defEVec = { 0, -500 };


	// 構造体定義
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
	//標準オブジェクト
	HDC			hdc;			// デバイスコンテキストハンドル
	//HBITMAP		hDstBmp;		// ビットマップハンドル
	PAINTSTRUCT	ps;				// クライアント領域に描画する時に必要な構造体
	static int fire_num = 0;
	//int			i;
	static unsigned short int mouse_x = -10, mouse_y = 10;
	_TCHAR str[256];


	switch (uMsg)
	{
	case WM_CREATE:
	{
		// 変数初期化
		srand((unsigned int)time(NULL));
		SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//第3引数:50ms(1000/20fps)
		InitInput();

		//インスタンスの生成
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
		//直接生成

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
		// 時間の経過
		g_nTime++;
		// 線形補間計算
		FLOAT deltaT =
			(g_nTime - root[g_nRootIdx].t0) /
			(FLOAT)(root[g_nRootIdx].t1 - root[g_nRootIdx].t0);
		g_pos.x =
			root[g_nRootIdx].start.x +
			(LONG)((root[g_nRootIdx].end.x - root[g_nRootIdx].start.x) * deltaT);
		g_pos.y =
			root[g_nRootIdx].start.y +
			(LONG)((root[g_nRootIdx].end.y - root[g_nRootIdx].start.y) * deltaT);

		// ルートの更新
		if (g_nTime >= root[g_nRootIdx].t1)	//終点を超えたら次のIdxへ
		{
			g_nRootIdx++;
			if (root[g_nRootIdx].start.x == -1)	// 
			{	// 終点なので初期化
				g_nRootIdx = 0;
				g_nTime = 0;
			}
		}

		UpdateInput();
		//// 自機を操作
		//if (GetAsyncKeyState(VK_W)) player_y -= MOVE_SPEED;

		//Fireworks生成
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

		// 再描画を要求
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
			int ret = MessageBox(hWnd, _T("終了しますか?"), _T("終了確認"), MB_OKCANCEL);
			if (ret == IDOK) {
				DestroyWindow(hWnd);	// ウィンドウ破棄を指示
			} else {
				SetTimer(hWnd, IDT_TIMER1, 1000 / FPS, NULL);
			}
			return 0;
		}
		break;
	}
	case WM_PAINT:		//描画命令が出た
	{
		//描画開始
		hdc = BeginPaint(hWnd, &ps);
		// 描画前に画面クリア
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

		//地面
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

		//// 制御点
		//wsprintf(str, TEXT("WASD : プレイヤー"));
		//TextOut(hdc, 5, 5 + tm.tmHeight * 0, str, _tcslen(str));
		//wsprintf(str, TEXT("SPACE : 発射"));
		//TextOut(hdc, 5, 5 + tm.tmHeight * 1, str, _tcslen(str));
		//wsprintf(str, TEXT("↑←↓→ : 敵"));
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

		//描画終了
		EndPaint(hWnd, &ps);
		break;
		
	}
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:							// 終了指示がきた
	{
		UninitInput();
		KillTimer(hWnd, IDT_TIMER1);			// タイマーを停止
		//インスタンスの削除
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

		PostQuitMessage(0);						// システムにスレッドの終了を要求
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

	// ウィンドウ作成
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
		sizeof(WNDCLASSEX),				// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,		// クラスのスタイル
		WindowProc,						// ウィンドウプロシージャ
		0,								// 補助メモリ
		0,								// 補助メモリ
		GetModuleHandle(NULL),			// このプログラムのインスタンスハンドル
		LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)),// アイコン画像
		LoadCursor(NULL, IDC_ARROW),	// カーソル画像
		NULL,							// 背景ブラシ(背景色)
		NULL,							// メニュー名
		Window::ClassName,				// クラス名									
		NULL							// 小さいアイコン
	};

	// 構造体の登録
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
