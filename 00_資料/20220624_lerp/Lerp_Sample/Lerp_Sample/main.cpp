#include <Windows.h>

#pragma comment(lib, "winmm.lib")

// マクロ定義
#define CLASS_NAME	"サンプル"
#define WINDOW_NAME	"線形補間演習"

#define SCREEN_WIDTH	(640)	// ウィンドウの幅
#define SCREEN_HEIGHT	(480)	// ウィンドウの高さ

// 構造体定義
typedef struct _tRootData
{
	POINT	start;	// 
	POINT	end;	// 
	int		t0;		// 
	int		t1;		// 
}tRootData;

// プロトタイプ宣言
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);

// グローバル変数
HINSTANCE g_hInstance;
int g_nTime = 0;	// 時間
POINT g_pos = { 100,100 };	// 円の位置
tRootData root[] = {
	
	
	
	
	{-1},
};
int	g_nRootIdx = 0;

// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
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
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
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

	// タイマーのセット(第3引数がタイマー間隔ミリ秒)
	SetTimer(hWnd, 1, 20, NULL);

	// メッセージループ
	while (GetMessage(&msg, NULL, 0, 0) != 0)
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
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	switch (uMsg)
	{
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		// 時間の経過
		
		// 線形補間計算
		
		
		// ルートの更新
		if (g_nTime >= root[g_nRootIdx].t1)
		{
			g_nRootIdx++;
			if (root[g_nRootIdx].start.x == -1)	// 
			{	// 終点なので初期化
				g_nRootIdx = 0;
				g_nTime = 0;
			}
		}
		// 再描画指示
		InvalidateRect(hWnd, &rect, TRUE);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			// 変数の再初期化
			break;
		}
		break;

	case WM_PAINT:
	{
		HDC			hDC;
		PAINTSTRUCT	ps;
		char		aStr[256];

		// 描画の開始
		hDC = BeginPaint(hWnd, &ps);

		// テキスト表示
		SetTextColor(hDC, RGB(255, 0, 0));
		wsprintf(aStr, "SPACEで再初期化");
		TextOut(hDC, 10, 10, aStr, (int)strlen(aStr));

		// 線を描画
		for (int i = 0; i < sizeof(root)/sizeof(tRootData) - 1; i++)
		{
			
			
		}

		// 楕円を描画
		// ペンを作成
		HPEN hPen;		// 現在のペン
		HPEN hOldPen;	// バックアップ用
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		// ペンを持ち替える
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		// 描画
		Ellipse(hDC, g_pos.x - 20, g_pos.y - 20, g_pos.x + 20, g_pos.y + 20);
		// ペンをもとに戻す
		SelectObject(hDC, hOldPen);
		// 作成したペンを削除
		DeleteObject(hPen);

		// 描画の終了
		EndPaint(hWnd, &ps);
	}
	break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
