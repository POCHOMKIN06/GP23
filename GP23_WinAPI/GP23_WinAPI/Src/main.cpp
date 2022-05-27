//課題05　運動してる?
//2:投下速度運動、3:自由落下運動、5:斜方投射を画面内に表現する
//提出期限:5/13(金)

// ヘッダー
#include "main.h"

#include <iostream>
#include <cstdio>
#include "strsafe.h"

#include "Window.h"




//マクロ定義
#define IDT_TIMER1	(100)

// プロトタイプ宣言

// グローバル変数
HINSTANCE g_hInstance;

using namespace Oshima;

// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	Window window(Window::ClassName, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window.Create() == FALSE) {
		return 0;
	}

	HWND	hWnd = window.GetWinHandle();
	MSG		msg;

	SetTimer(hWnd, IDT_TIMER1, 1000/FPS, NULL);	//第3引数:50ms(1000/20fps)

	// メッセージループ
 	while (GetMessage(&msg, NULL, 0, 0 ) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
