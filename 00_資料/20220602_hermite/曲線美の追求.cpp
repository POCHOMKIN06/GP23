//=================================================================================================
//
//	  ＧＰ２３　演習「曲線美の追求」　ベースプログラム
//
//=================================================================================================

//-------- インクルードヘッダ
#define STRICT				// Windowsプログラムに対する「型保証」を強化
#include <windows.h>		// Windowsプログラムにはこれを付ける
#include <tchar.h>			// 汎用テキスト マッピング

//-------- 定数定義
#define CLASS_NAME		_T("GP23_Hermite2")		// ウィンドウクラス名
#define CAPTION_NAME	_T("[ GP23 Hermite2 ]")	// キャプション名
const UINT	SCREEN_WIDTH	= 640;				// スクリーン＆ビットマップ幅
const UINT	SCREEN_HEIGHT	= 480;				// スクリーン＆ビットマップ高さ

enum _bmp {
	BMP_BG = 0,		// 0
	BMP_PLAYER,		// 1
	BMP_ENEMY,		// 2
	BMP_QTY,		// 3 BMPの総数を示す
};

LPCTSTR		bmp_file[] = {
	_T("bg.bmp"),		// 0 ＢＧ
	_T("player.bmp"),	// 1 自機
	_T("enemy.bmp"),	// 2 敵機
};

const UINT	MOVE_SPEED	= 8;				// 自機移動スピード
const UINT	BG_W		= 640;				// ＢＧ　幅
const UINT	BG_H		= 480;				// ＢＧ　高さ
const UINT	PLAYER_W	= 48;				// 自機　幅
const UINT	PLAYER_H	= 32;				// 自機　高さ
const UINT	ENEMY_W		= 32;				// 敵機　幅
const UINT	ENEMY_H		= 32;				// 敵機　高さ
const UINT	CURVE_DIV	= 10;				// 曲線分割数

const float	PLAYER_VECTOR_X	= -500.0f;		// 自機側　接線ベクトル　Ｘ成分
const float	PLAYER_VECTOR_Y	= -500.0f;		// 自機側　接線ベクトル　Ｙ成分
const float	ENEMY_VECTOR_X	=  300.0f;		// 敵機側　接線ベクトル　Ｘ成分
const float	ENEMY_VECTOR_Y	=  200.0f;		// 敵機側　接線ベクトル　Ｙ成分

//-------- 構造体定義
struct LASER {
	int		status;							// ステータス
	int		start;							// レーザー開始インデックス
	int		end;							// レーザー終了インデックス
	POINT	curve_pt[CURVE_DIV + 1];		// エルミート曲線上の座標
};

//-------- グローバル変数定義
HINSTANCE	g_hInst;						// インスタンスハンドル
HDC			g_hMemDC[BMP_QTY];				// メモリＤＣ（ＢＭＰ保持用）
int			player_x, player_y;				// 自機座標
int			enemy_x , enemy_y;				// 敵機座標

LASER		laser;							// レーザー

//-------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------------------------------------------
//	メイン
//-------------------------------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	HWND	hWnd;		// ウィンドウハンドル
	MSG		msg;		// メッセージ情報

	g_hInst = hInstance;

	// ウィンドウクラスの登録
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc,
		0, 0, hInstance, LoadIcon(hInstance, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(WHITE_BRUSH), NULL, CLASS_NAME, NULL};
	if (RegisterClassEx(&wc) == 0) return false;	// ウィンドウクラスを登録

	// ウィンドウを作成
	hWnd = CreateWindowEx(
		WS_EX_TOPMOST, CLASS_NAME, CAPTION_NAME, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT,
		SCREEN_WIDTH  + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
		SCREEN_HEIGHT + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) return false;
	ShowWindow(hWnd, iCmdShow);					// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);							// ウィンドウのクライアント領域を更新

	// メッセージループ
	msg.message = WM_NULL;
	while (WM_QUIT != msg.message) {					// WM_QUIT がくるまでループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {	// メッセージをチェック
			TranslateMessage(&msg);						// 仮想キーメッセージを文字メッセージへ変換
			DispatchMessage(&msg);						// ウィンドウプロシージャへメッセージを送出
		}
	}
	return msg.wParam;
}

//-------------------------------------------------------------------------------------------------
//	ウィンドウプロシージャ
//-------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;			// デバイスコンテキストハンドル
	HBITMAP		hDstBmp;		// ビットマップハンドル
	PAINTSTRUCT	ps;
	HPEN		hOldPen;
	int			i;

	switch (uMsg) {						// メッセージをチェック
	case WM_CREATE:
		// ビットマップファイルをロード
		hdc = GetDC(hWnd);
		for (i = 0; i < BMP_QTY; i++) {
			hDstBmp = (HBITMAP)LoadImage(g_hInst, bmp_file[i], IMAGE_BITMAP, 0, 0,
				LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			g_hMemDC[i] = CreateCompatibleDC(hdc);				// メモリＤＣを作成
			SelectObject(g_hMemDC[i], hDstBmp);					// ビットマップを選択
			DeleteObject(hDstBmp);								// ロードしたビットマップを破棄
		}
		ReleaseDC(hWnd, hdc);

		// 変数初期化
		player_x = 120;
		player_y = 240;
		enemy_x  = 550;
		enemy_y  = 240;
		laser.status = 0;

		SetTimer(hWnd, 1, 20, NULL);		// タイマーを起動
		return 0;
	case WM_KEYDOWN:							// キーボードが押された
		switch (wParam) {							// パラメータをチェック
		case VK_ESCAPE:								// [ESC]キーが押された
			DestroyWindow(hWnd);					// ウィンドウ破棄を指示
			return 0;
		}
		break;
	case WM_TIMER:
		// 自機を操作
		if (GetAsyncKeyState(VK_UP   )) player_y -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_DOWN )) player_y += MOVE_SPEED;
		if (GetAsyncKeyState(VK_LEFT )) player_x -= MOVE_SPEED;
		if (GetAsyncKeyState(VK_RIGHT)) player_x += MOVE_SPEED;
		if (GetAsyncKeyState(VK_SPACE) && laser.status == 0) laser.status = 1;	// レーザーを発射
		// 再描画を要求
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, 0       , 0       , BG_W    , BG_H    , g_hMemDC[BMP_BG    ], 0, 0, SRCCOPY);	// ＢＧ描画
		BitBlt(hdc, player_x, player_y, PLAYER_W, PLAYER_H, g_hMemDC[BMP_PLAYER], 0, 0, SRCCOPY);	// 自機描画
		BitBlt(hdc, enemy_x , enemy_y , ENEMY_W , ENEMY_H , g_hMemDC[BMP_ENEMY ], 0, 0, SRCCOPY);	// 敵機描画

		// レーザーを描画
		hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(0, 255, 255)));	// レーザー用ペンを作成

		//----------------------------------------------------------------
		//  ここにエルミート曲線を使った式を書いて
		//  曲線レーザーを描画させる
		//----------------------------------------------------------------

		DeleteObject(SelectObject(hdc, hOldPen));	// 作成したペンを削除

		EndPaint(hWnd, &ps);
		return 0;
	case WM_CLOSE:
		for (i = 0; i < BMP_QTY; i++) {
			DeleteDC(g_hMemDC[i]);				// メモリＤＣを削除
		}
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:							// 終了指示がきた
		KillTimer(hWnd, 1);							// タイマーを停止
		PostQuitMessage(0);							// システムにスレッドの終了を要求
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定ウィンドウプロシージャを呼出
}

//=================================================================================================
//	end
//=================================================================================================
