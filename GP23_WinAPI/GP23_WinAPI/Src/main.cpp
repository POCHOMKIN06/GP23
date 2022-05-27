#include <Windows.h>
#include <tchar.h>

#pragma comment(lib, "winmm.lib")

// マクロ定義
#define CLASS_NAME	TEXT("サンプル")
#define WINDOW_NAME	TEXT("Window01")

#define SCREEN_WIDTH	(640)	// ウィンドウの幅
#define SCREEN_HEIGHT	(640)	// ウィンドウの高さ

// プロトタイプ宣言
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);

// グローバル変数
HINSTANCE g_hInstance;
POINT g_pos = { 100, 200 };
POINT g_size = { 200, 200 };
POINT g_move = { 4,4 };

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

	SetTimer(hWnd, 100, 50, NULL);	//ms, 20fps

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
	HDC hDC;			//デバイスコンテキストのハンドル
	PAINTSTRUCT ps;		//クライアント領域に描画する時に必要な構造体
	TCHAR str[256];		//文字列表示用
	HPEN hPen;			//ペンオブジェクト
	HPEN hPenOld;		//変更前のオブジェクト保存用
	HBRUSH hBrush;		//ブラシオブジェクト
	HBRUSH hBrushOld;	//変更前のオブジェクト保存用


	switch(uMsg)
	{
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);
		break;
	case WM_TIMER:
	{
		g_pos.x += 2;

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

		//描画処理はここに書く
		//テキストを表示
		wsprintf(str, TEXT("TEST"));	//バッファに格納
		TextOut(hDC, 100, 100, str, _tcslen(str));	//表示座標,文字列と長さ

		SetTextColor(hDC, RGB(255, 0, 0));			//テキスト色の設定
		TextOut(hDC, 120, 105, str, _tcslen(str));	//表示座標,文字列と長さ

		//線を描画(始点->次の点->その次の点)
		MoveToEx(hDC, 400, 100, NULL);
		LineTo(hDC, 500, 200);
		LineTo(hDC, 400, 200);

		//ペンを作成
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));	//線種,太さ,色
		//ペンを持ち替え
		hPenOld = (HPEN)SelectObject(hDC, hPen);

		//円を描画
		Ellipse(hDC,
			g_pos.x - g_size.x / 2,
			g_pos.y - g_size.y / 2,
			g_pos.x + g_size.x / 2,
			g_pos.y + g_size.y / 2
		);

		//ペンを戻す
		SelectObject(hDC, hPenOld);
		//作成したペンの削除
		DeleteObject(hPen);


		//ブラシの作成
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		//ブラシを持ち替え
		hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);

		//四角を描画
		Rectangle(hDC, 340, 340, 460, 460);

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
			g_pos.y -= g_move.y;
			break;
		case VK_DOWN:
			g_pos.y += g_move.y;
			break;
		case VK_RIGHT:
			g_pos.x += g_move.x;
			break;
		case VK_LEFT:
			g_pos.x -= g_move.x;
			break;
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
