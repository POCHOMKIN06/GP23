#include "Window.h"

#include "BegieCurve.h"

const TCHAR* Window::ClassName = TEXT("サンプル");

namespace{
POINTFLOAT g_pos[4] = {
	{100.0f, 400.0f},	//始点
	{200.0f, 100.0f},	//中間点
	{500.0f, 200.0f},	//中間点
	{600.0f, 550.0f}	//終点
};
}
BegieCurve BCurve;

LRESULT Window::WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{	
	//標準オブジェクト
	HDC hDC;			//デバイスコンテキストのハンドル
	PAINTSTRUCT ps;		//クライアント領域に描画する時に必要な構造体

	switch (message_id)
	{
	case WM_CREATE:
	{
		BCurve.SetControlPoint(g_pos[0], g_pos[1], g_pos[2], g_pos[3], FALSE);
		break;
	}
	case WM_PAINT:		//描画命令が出た
	{
		//描画開始
		hDC = BeginPaint(window_handle, &ps);

		//
		//描画処理はここに書く
		//

		//制御点の描画
		BCurve.DrawControlPoint(hDC);

		//曲線を描画
		BCurve.Draw(hDC);

		//描画終了
		EndPaint(window_handle, &ps);
		break;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
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
