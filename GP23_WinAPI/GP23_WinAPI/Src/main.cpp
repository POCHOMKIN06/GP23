//実習　ベクトルの表示

// ヘッダー
#include "main.h"

#include <iostream>
#include <cstdio>
#include "strsafe.h"


#include "AxisLine.h"
#include "VIewer.h"
#include "MoveType.h"
#include "MovingCircle.h"
#include "Vector.h"

//マクロ定義
#define IDT_TIMER1	(100)

// プロトタイプ宣言
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);

//パラメータプリセット
//constexpr float INIT_VEL     = 10.0f;
//constexpr float ACCELERATION = 4.0f;
//constexpr float GRAVITY      = -9.8f;

// グローバル変数
HINSTANCE g_hInstance;
//float g_time;
//float g_initvel;
//float g_acc;
//float g_gravityAcc;
//float g_theta;
POINT g_origin = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

//自作オブジェクト
AxisLine		g_AxisLine(g_origin.x, g_origin.y);		//座標軸
//Viewer			g_Viewer;								//各種パラメータ表示
//MOVE_TYPE		g_MoveType = MOVE_TYPE::Constant_Velocity;				//運動挙動の種類
//MovingCircle	g_Circle[static_cast<unsigned int>(MOVE_TYPE::END)];	//運動する円
Vector			g_Vector[10];


// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;


	WNDCLASSEX wcex = {
		sizeof( WNDCLASSEX ),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor( NULL, IDC_ARROW ),	//カーソルアイコン読込
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};

	HWND	hWnd;
	MSG		msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,	//作成位置(x,y)
		CW_USEDEFAULT,	//左上(0,0)
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//第3引数:50ms(1000/20fps)

	// メッセージループ
	while (GetMessage(&msg, NULL, 0, 0 ) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ウィンドウの登録を破棄
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam)
{
	//標準オブジェクト
	HDC hDC;			//デバイスコンテキストのハンドル
	PAINTSTRUCT ps;		//クライアント領域に描画する時に必要な構造体
	HPEN hPen;			//ペンオブジェクト
	HPEN hPenOld;		//変更前のオブジェクト保存用
	HBRUSH hBrush;		//ブラシオブジェクト
	HBRUSH hBrushOld;	//変更前のオブジェクト保存用


	switch(uMsg)
	{
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		g_Vector[0] = Vector(60, 90);
		break;
	case WM_TIMER:
	{
		//再描画を指示
		RECT rect = {
			0,0, SCREEN_WIDTH, SCREEN_HEIGHT
		};
		InvalidateRect(hWnd, &rect, TRUE);

		break;
	}
	case WM_PAINT:		//描画命令が出た
	{
		//描画開始
		hDC = BeginPaint(hWnd, &ps);

		//
		//描画処理はここに書く
		//

		//座標軸作成
		g_AxisLine.Draw(hDC);
		//表示物
		g_Vector[0].Draw(hDC, g_origin);


		//ペンを作成
		hPen = CreatePen(PS_SOLID, 3, GREEN);	//線種,太さ,色
		//ペンを持ち替え
		hPenOld = (HPEN)SelectObject(hDC, hPen);

		//円を描画

		//ペンを戻す
		SelectObject(hDC, hPenOld);
		//作成したペンの削除
		DeleteObject(hPen);

		//ブラシの作成
		hBrush = CreateSolidBrush(RED);
		//ブラシを持ち替え
		hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);

		//四角を描画
		//Rectangle(hDC, 340, 340, 460, 460);
		//ブラシを戻す
		SelectObject(hDC, hBrushOld);
		//作成したブラシの削除
		DeleteObject(hBrush);

		//描画終了
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:	//キーが押下された
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			int id = MessageBox(hWnd,
				TEXT("終了しますか?"),
				TEXT("確認"),
				MB_YESNO);
			if (id == IDYES) {
				DestroyWindow(hWnd);
			}
		}
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		case VK_RIGHT:
			break;
		case VK_LEFT:
			break;
		case VK_NUMPAD1:
			break;
		case VK_NUMPAD2:
			break;
		case VK_NUMPAD3:
			break;
		case VK_NUMPAD4:
			break;
		case VK_NUMPAD5:
			break;
		case VK_SPACE:
			break;
		default:
			break;
		}
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

