// ヘッダー
#include "main.h"

#include <iostream>
#include <cstdio>
#include "strsafe.h"

#include "Window.h"
#include "Window2.h"

// グローバル変数
HINSTANCE g_hInstance;

// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	Window window(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window.Create() == FALSE) {
		return 0;
	}
	//HWND hWnd = window.GetWinHandle();
	//SetWindowPos(hWnd, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
	
	//Window2 window2(TEXT("window2"), SCREEN_WIDTH, SCREEN_HEIGHT);
	//if (window2.Create() == FALSE) {
	//	return 0;
	//}
	//HWND hWnd2 = window2.GetWinHandle();
	//SetWindowPos(hWnd2, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);



	MSG msg;
	// メッセージループ
	msg.message = WM_NULL;
	while (WM_QUIT != msg.message) {					// WM_QUIT がくるまでループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {	// メッセージをチェック
			TranslateMessage(&msg);						// 仮想キーメッセージを文字メッセージへ変換
			DispatchMessage(&msg);						// ウィンドウプロシージャへメッセージを送出
		} else {

		}
	}
	return msg.wParam;
}

