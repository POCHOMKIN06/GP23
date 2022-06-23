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

const TCHAR* Window::ClassName = TEXT("サンプル");

//マクロ定義
#define IDT_TIMER1	(100)

namespace
{

	Fireworks* Fires[50];
	NightSky Sky;
	StarLight* Stars[100];
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//標準オブジェクト
	HDC			hdc;			// デバイスコンテキストハンドル
	HBITMAP		hDstBmp;		// ビットマップハンドル
	PAINTSTRUCT	ps;				// クライアント領域に描画する時に必要な構造体
	int fire_num = 0;
	int			i;
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
		//// 自機を操作
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

		// 制御点
		wsprintf(str, TEXT("WASD : プレイヤー"));
		TextOut(hdc, 5, 5 + tm.tmHeight * 0, str, _tcslen(str));
		wsprintf(str, TEXT("SPACE : 発射"));
		TextOut(hdc, 5, 5 + tm.tmHeight * 1, str, _tcslen(str));
		wsprintf(str, TEXT("↑←↓→ : 敵"));
		TextOut(hdc, 200, 5 + tm.tmHeight * 0, str, _tcslen(str));
		SetBkMode(hdc, OPAQUE);
		SetTextColor(hdc, DefaultC);

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
		for (int i = 0; i < 50; i++) {
			delete Fires[i];
		}
		for (int i = 0; i < 100; i++) {
			delete Stars[i];
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
