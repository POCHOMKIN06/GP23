#include <Windows.h>

#pragma comment(lib, "winmm.lib")

// マクロ定義
#define CLASS_NAME	"サンプル"
#define WINDOW_NAME	"Window01"

#define SCREEN_WIDTH	(320)	// ウィンドウの幅
#define SCREEN_HEIGHT	(320)	// ウィンドウの高さ

// プロトタイプ宣言
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);

// グローバル変数
HINSTANCE g_hInstance;


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
		LoadCursor( NULL, IDC_ARROW ),
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
	switch(uMsg)
	{
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
