#include "Window.h"

#include "main.h"
#include "BegieCurve.h"

const TCHAR* Window::ClassName = TEXT("サンプル");

//マクロ定義
#define IDT_TIMER1	(100)

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
const UINT	BG_W = 640;				// ＢＧ　幅
const UINT	BG_H = 480;				// ＢＧ　高さ
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
struct LASER {
	int		status;							// ステータス
	int		start;							// レーザー開始インデックス
	int		end;							// レーザー終了インデックス
	POINTFLOAT	curve_pt[CURVE_DIV + 1];	// エルミート曲線上の座標
};

BegieCurve BCurve;

HDC			g_hMemDC[BMP_QTY];				// メモリＤＣ（ＢＭＰ保持用）
int			player_x, player_y;				// 自機座標
int			enemy_x, enemy_y;				// 敵機座標

LASER		laser;							// レーザー



LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//標準オブジェクト
	HDC			hdc;			// デバイスコンテキストハンドル
	HBITMAP		hDstBmp;		// ビットマップハンドル
	PAINTSTRUCT	ps;				//クライアント領域に描画する時に必要な構造体
	HPEN		hOldPen;
	int			i;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		BCurve.SetControlPoint(g_pos[0], g_pos[1], g_pos[2], g_pos[3], FALSE);

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
		laser.status = 0;

		SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//第3引数:50ms(1000/20fps)

		return 0;
	}
	case WM_TIMER:
	{
		// 自機を操作
		if (GetAsyncKeyState(VK_UP)) player_y -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_DOWN)) player_y += MOVE_SPEED;
		if (GetAsyncKeyState(VK_LEFT)) player_x -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_RIGHT)) player_x += MOVE_SPEED;
		if (GetAsyncKeyState(VK_SPACE) && laser.status == 0) laser.status = 1;	// レーザーを発射
		// 再描画を要求
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
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
		// 制御点の描画
		BCurve.DrawControlPoint(hdc);
		// 曲線を描画
		BCurve.Draw(hdc);


		BitBlt(hdc, 0, 0, BG_W, BG_H, g_hMemDC[BMP_BG], 0, 0, SRCCOPY);	// ＢＧ描画
		BitBlt(hdc, player_x, player_y, PLAYER_W, PLAYER_H, g_hMemDC[BMP_PLAYER], 0, 0, SRCCOPY);	// 自機描画
		BitBlt(hdc, enemy_x, enemy_y, ENEMY_W, ENEMY_H, g_hMemDC[BMP_ENEMY], 0, 0, SRCCOPY);	// 敵機描画

		// レーザーを描画
		hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(0, 255, 255)));	// レーザー用ペンを作成


		//----------------------------------------------------------------
		//  ここにエルミート曲線を使った式を書いて
		//  曲線レーザーを描画させる
		//----------------------------------------------------------------
		float h00, h01, h10, h11;
		float t;

		//エルミート曲線を計算
		for (i = 0; i <= CURVE_DIV; i++) {
			t = (float)i / CURVE_DIV;
			h00 = (t - 1)*(t - 1)*(2 * t + 1);		// エルミート補間関数H0,0
			h01 = t * t*(3 - 2 * t);				// エルミート補間関数H0,1
			h10 = (1 - t * t)*t;					// エルミート補間関数H1,0
			h11 = (t - 1)*t*t;						// エルミート補間関数H1,1
			laser.curve_pt[i].x = h00 * player_x + h01 * enemy_x + h10 * PLAYER_VECTOR_X + h11 * ENEMY_VECTOR_X;
			laser.curve_pt[i].y = h00 * player_y + h01 * enemy_y + h10 * PLAYER_VECTOR_Y + h11 * ENEMY_VECTOR_Y;
		}
		//描画用始点と終点を設定
		switch (laser.status)
		{
		case 0:
			laser.start = 0;
			laser.end = 0;
			break;
		case 1:
			if (laser.end < CURVE_DIV) {	// レーザーを敵まで伸ばす
				laser.end++;
			} else {	// 次へ
				laser.status = 2;
			}
			break;
			//laser.start = 0;
			//laser.end = CURVE_DIV;
			//break;
		default:
			break;
		}

		//エルミート曲線を描画
		MoveToEx(hdc, static_cast<int>(laser.curve_pt[laser.start].x), static_cast<int>(laser.curve_pt[laser.start].y), NULL);
		for (i = laser.start + 1; i <= laser.end; i++) {
			LineTo(hdc, static_cast<int>(laser.curve_pt[i].x), static_cast<int>(laser.curve_pt[i].y));
		}

		//描画終了
		DeleteObject(SelectObject(hdc, hOldPen));	// 作成したペンを削除
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
