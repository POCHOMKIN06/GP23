#include "Window.h"

#include "main.h"
#include "input.h"

#include "BegieCurve.h"
#include "HermitianCurve.h"
#include "Laser.h"
#include "MovableHermitianCurve.h"

const TCHAR* Window::ClassName = TEXT("サンプル");

//マクロ定義
#define IDT_TIMER1	(100)
namespace
{
	POINTFLOAT g_pos[4] = {
		{100.0f, 400.0f},	//始点
		{200.0f, 100.0f},	//中間点
		{500.0f, 200.0f},	//中間点
		{600.0f, 550.0f}	//終点
	};


	enum _bmp {
		BMP_BG = 0,		// 0
		BMP_PLAYER,		// 1
		BMP_ENEMY,		// 2
		BMP_QTY,		// 3 BMPの総数を示す
	};

	LPCTSTR		bmp_file[] = {
		_T("Bmp/bg.bmp"),		// 0 ＢＧ
		_T("Bmp/player.bmp"),	// 1 自機
		_T("Bmp/enemy.bmp"),	// 2 敵機
	};

	const UINT	MOVE_SPEED = 8;				// 自機移動スピード
	const UINT	BG_W = 640;					// ＢＧ　幅
	const UINT	BG_H = 480;					// ＢＧ　高さ
	const UINT	PLAYER_W = 48;				// 自機　幅
	const UINT	PLAYER_H = 32;				// 自機　高さ
	const UINT	ENEMY_W = 32;				// 敵機　幅
	const UINT	ENEMY_H = 32;				// 敵機　高さ
	const UINT	CURVE_DIV = 10;				// 曲線分割数

	const float	PLAYER_VECTOR_X = -500.0f;		// 自機側　接線ベクトル　Ｘ成分
	const float	PLAYER_VECTOR_Y = -500.0f;		// 自機側　接線ベクトル　Ｙ成分
	const float	ENEMY_VECTOR_X = 300.0f;		// 敵機側　接線ベクトル　Ｘ成分
	const float	ENEMY_VECTOR_Y = 200.0f;		// 敵機側　接線ベクトル　Ｙ成分

	//-------- 構造体定義
	//struct LASER {
	//	int		status;							// ステータス
	//	int		start;							// レーザー開始インデックス
	//	int		end;							// レーザー終了インデックス
	//	POINTFLOAT	curve_pt[CURVE_DIV + 1];	// エルミート曲線上の座標
	//};

	//BegieCurve BCurve;

	HDC			g_hMemDC[BMP_QTY];				// メモリＤＣ（ＢＭＰ保持用）
	int			player_x, player_y;				// 自機座標
	int			enemy_x, enemy_y;				// 敵機座標

	//LASER		laser;							// レーザー

	Laser laser;
	static MovableHermitianCurve MHCurve1;

	int at;
}
LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//標準オブジェクト
	HDC			hdc;			// デバイスコンテキストハンドル
	HBITMAP		hDstBmp;		// ビットマップハンドル
	PAINTSTRUCT	ps;				//クライアント領域に描画する時に必要な構造体
	int			i;
	static unsigned short int mouse_x = -10, mouse_y = 10;
	TCHAR str[256];
	static int check = -1;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		//BCurve.SetControlPoint(g_pos[0], g_pos[1], g_pos[2], g_pos[3], FALSE);

		// ビットマップファイルをロード
		hdc = GetDC(hWnd);
		for (i = 0; i < BMP_QTY; i++) {
			hDstBmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), bmp_file[i], IMAGE_BITMAP, 0, 0,
				LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			g_hMemDC[i] = CreateCompatibleDC(hdc);				// メモリＤＣを作成
			SelectObject(g_hMemDC[i], hDstBmp);					// ビットマップを選択
			DeleteObject(hDstBmp);								// ロードしたビットマップを破棄
		}
		ReleaseDC(hWnd, hdc);

		// 変数初期化
		player_x = 120;
		player_y = 240;
		enemy_x = 550;
		enemy_y = 240;

		at = 270;

		if (laser.Create(10,
			POINTFLOAT{ (float)player_x, (float)player_y }, POINTFLOAT{ PLAYER_VECTOR_X, PLAYER_VECTOR_Y },
			POINTFLOAT{ (float)enemy_x,  (float)enemy_y }, POINTFLOAT{ ENEMY_VECTOR_X, ENEMY_VECTOR_Y })
			== FALSE) {
			MessageBox(hWnd, TEXT("laserの生成に失敗"), TEXT("生成エラー"), MB_OK);
			PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}

		if (MHCurve1.Create(20,
			POINTFLOAT{ 100.0f, 340.0f }, POINTFLOAT{ 200.0f, 100.0f },
			POINTFLOAT{ 320.0f,  80.0f }, POINTFLOAT{ 200.0f, 100.0f })
			== FALSE) {
			MessageBox(hWnd, TEXT("MHCurveの生成に失敗"), TEXT("生成エラー"), MB_OK);
			PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}


		SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//第3引数:50ms(1000/20fps)

		return 0;
	}
	case WM_TIMER:
	{
		// 自機を操作
		if (GetAsyncKeyState(VK_W)) player_y -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_S)) player_y += MOVE_SPEED;
		if (GetAsyncKeyState(VK_A)) player_x -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_D)) player_x += MOVE_SPEED;
		// 敵機を操作
		if (GetAsyncKeyState(VK_UP))	enemy_y -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_DOWN))	enemy_y += MOVE_SPEED;
		if (GetAsyncKeyState(VK_LEFT))	enemy_x -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_RIGHT))	enemy_x += MOVE_SPEED;

		laser.Update(POINTFLOAT{ (float)player_x, (float)player_y }, POINTFLOAT{ (float)enemy_x, (float)enemy_y });

		using LS = Laser::STATUS;
		if (GetAsyncKeyState(VK_SPACE) && laser.GetStatus() == LS::STANDBY) laser.SetStatus(LS::SHOOT);	// レーザーを発射

		at -= 5;
		if (at <= 0) {
			at = 270;
		}
		// 再描画を要求
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
			if (wParam & MK_LBUTTON) {

				mouse_x = LOWORD(lParam);
				mouse_y = HIWORD(lParam);


				if (MHCurve1.CheckMousePos(mouse_x, mouse_y, &check)) {
					MHCurve1.MovePos(POINTFLOAT{ (float)mouse_x, (float)mouse_y }, check);
				}
			}
		break;
	}
	case WM_LBUTTONUP:
	{
			check = -1;
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



		//------------------------------------------
		// ベジエ曲線の描画
		//------------------------------------------
		//// 制御点の描画
		//BCurve.DrawControlPoint(hdc);
		//// 曲線を描画
		//BCurve.Draw(hdc);


		BitBlt(hdc, 0, 0, BG_W, BG_H, g_hMemDC[BMP_BG], 0, 0, SRCCOPY);	// ＢＧ描画
		BitBlt(hdc, player_x, player_y, PLAYER_W, PLAYER_H, g_hMemDC[BMP_PLAYER], 0, 0, SRCCOPY);	// 自機描画
		BitBlt(hdc, enemy_x, enemy_y, ENEMY_W, ENEMY_H, g_hMemDC[BMP_ENEMY], 0, 0, SRCCOPY);	// 敵機描画

		// レーザーを描画

		//----------------------------------------------------------------
		//  ここにエルミート曲線を使った式を書いて
		//  曲線レーザーを描画させる
		//----------------------------------------------------------------
		laser.Draw(hdc);
		
		MHCurve1.Draw(hdc);

		//HPEN old = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0)));
		//MoveToEx(hdc, 100+50 * cos(45), 100-50 * sin(45), NULL);
		//Rectangle(hdc, 210, 10, 400, 200);
		//HBRUSH oldb = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(0, 150, 150)));
		//AngleArc(hdc, 100, 100, 50, 45, at);
		//Pie(hdc, 210, 10, 400, 200, 0, 100, at, 0);
		//DeleteObject(SelectObject(hdc, old));
		//DeleteObject(SelectObject(hdc, oldb));
		
		//描画終了
		EndPaint(hWnd, &ps);
		break;
		
	}
	case WM_CLOSE:
	{
		for (i = 0; i < BMP_QTY; i++) {
			DeleteDC(g_hMemDC[i]);				// メモリＤＣを削除
		}
		DestroyWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:							// 終了指示がきた
	{
		KillTimer(hWnd, IDT_TIMER1);			// タイマーを停止
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
