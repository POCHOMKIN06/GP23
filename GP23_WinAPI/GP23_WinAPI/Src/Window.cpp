#include "Window.h"

#include "main.h"

const TCHAR* Oshima::Window::ClassName = TEXT("ぶつかってはじけろ！");

static POINTFLOAT	g_pos[2] = {
	{100.0f, SCREEN_HEIGHT / 2 + 100.0f},	// 1
	{100.0f, SCREEN_HEIGHT / 2 - 100.0f}	// 2
};
static POINTFLOAT	g_size = { 20.0f, 20.0f };	// サイズ
static POINTFLOAT	g_vel[2] = {	// 速度
	{5.0f, -5.0f},
	{5.0f, 5.0f}
};
static float	g_mass[2] = { 40.0f, 60.0f };	// 質量
#define BOUNCE_COEFFICIENT	(0.8f)			// 跳ね返り係数
static COLORREF	g_color[2] = {
	RGB(0,255,255),
	RGB(255,0,255)
};

LRESULT CALLBACK Oshima::Window::WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{	
	//標準オブジェクト
	HDC hDC;			//デバイスコンテキストのハンドル
	PAINTSTRUCT ps;		//クライアント領域に描画する時に必要な構造体
	char str[256];		// 文字列表示用
	HPEN hPen;			// ペンオブジェクト
	HPEN hPenOld;		// 変更前のペンオブジェクト保存用
	HBRUSH hBrush;		// ブラシオブジェクト
	HBRUSH hBrushOld;	// 変更前のブラシオブジェクト保存用

	switch (message_id)
	{
	case WM_TIMER:
	{
		for (int i = 0; i < 2; i++) {
			g_pos[i].x += g_vel[i].x;
			g_pos[i].y += g_vel[i].y;
		}

		// 当たり判定BC
		float fx = g_pos[0].x - g_pos[1].x;
		float fy = g_pos[0].y - g_pos[1].y;
		float flen = fx * fx + fy * fy;
		static bool isHit = false;	// 連続ヒット防止用
		if (isHit == false && flen <= (g_size.x / 2 + g_size.x / 2) * (g_size.x / 2 + g_size.x / 2)) {
			isHit = true;
			// 衝突後の速度計算
			POINTFLOAT v1New, v2New;
			v1New.x = ((g_mass[0] - BOUNCE_COEFFICIENT * g_mass[1]) * g_vel[0].x + (1.0f + BOUNCE_COEFFICIENT) * g_mass[1] * g_vel[1].x) / (g_mass[0] + g_mass[1]);
			v1New.y = ((g_mass[0] - BOUNCE_COEFFICIENT * g_mass[1]) * g_vel[0].y + (1.0f + BOUNCE_COEFFICIENT) * g_mass[1] * g_vel[1].y) / (g_mass[0] + g_mass[1]);
			v2New.x = ((g_mass[1] - BOUNCE_COEFFICIENT * g_mass[0]) * g_vel[1].x + (1.0f + BOUNCE_COEFFICIENT) * g_mass[0] * g_vel[0].x) / (g_mass[0] + g_mass[1]);
			v2New.y = ((g_mass[1] - BOUNCE_COEFFICIENT * g_mass[0]) * g_vel[1].y + (1.0f + BOUNCE_COEFFICIENT) * g_mass[0] * g_vel[0].y) / (g_mass[0] + g_mass[1]);

			// 格納
			g_vel[0].x = v1New.x;
			g_vel[0].y = v1New.y;
			g_vel[1].x = v2New.x;
			g_vel[1].y = v2New.y;
		} else {
			isHit = false;
		}

		// 再描画を指示
		RECT rect = { 0, 0, SCREEN_WIDTH,SCREEN_HEIGHT };
		InvalidateRect(window_handle, &rect, TRUE);

		break;
	}
	case WM_PAINT:		//描画命令が出た
	{
		//描画開始
		hDC = BeginPaint(window_handle, &ps);

		// テキストを表示
		wsprintf(str, "SPACEでリセット");	// バッファに格納
		//				座標		　文字列と長さ
		TextOut(hDC, 100, 100, str, strlen(str));
		// テキストを表示
		wsprintf(str, "→：次の運動、←：前の運動");	// バッファに格納
		//				座標		　文字列と長さ
		TextOut(hDC, 100, 120, str, strlen(str));

		//// テキストの色を変更
		//SetTextColor(hDC, RGB(255, 0, 0));
		//TextOut(hDC, 300, 200, str, strlen(str));

		// 中心
		MoveToEx(hDC, SCREEN_WIDTH / 2, 0, NULL);		// 始点
		LineTo(hDC, SCREEN_WIDTH / 2, SCREEN_HEIGHT);	// 次の点
		MoveToEx(hDC, 0, SCREEN_HEIGHT / 2, NULL);		// 始点
		LineTo(hDC, SCREEN_WIDTH, SCREEN_HEIGHT / 2);	// 終点

		for (int i = 0; i < 2; i++) {
			// ペンを作成
			//							太さ		色
			hPen = CreatePen(PS_SOLID, 3, g_color[i]);
			// ペンの持ち替え
			hPenOld = (HPEN)SelectObject(hDC, hPen);
			// 円を描画
			Ellipse(hDC,
				// 座標+-サイズの半分
				(int)(g_pos[i].x - g_size.x / 2),	// 左
				(int)(g_pos[i].y - g_size.y / 2),	// 上
				(int)(g_pos[i].x + g_size.x / 2),	// 右
				(int)(g_pos[i].y + g_size.y / 2));	// 下
			// ペンを戻す
			SelectObject(hDC, hPenOld);
			// 作成したペンの削除
			DeleteObject(hPen);
		}

		// ブラシ作成
		hBrush = CreateSolidBrush(RGB(0, 0, 255));	// 色
		// ブラシ持ち替え
		hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);
		//// 四角形を描画
		////				左	上	右	下
		//Rectangle(hDC, 340, 340, 460, 460);
		// ブラシを戻す
		SelectObject(hDC, hBrushOld);
		// 作成したブラシの削除
		DeleteObject(hBrush);


		//描画終了
		EndPaint(window_handle, &ps);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wparam) {
		case VK_SPACE:
			// リセット
			break;
		case VK_RIGHT:
			// リセット
			break;
		case VK_LEFT:
			// リセット
			break;
		case VK_ESCAPE:
			{
				int id = MessageBox(window_handle, "終了しますか？", "確認", MB_YESNO);
				if (id == IDYES) {
					DestroyWindow(window_handle);
				}
				break;
			}
		}
		break;
	}
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}

bool Oshima::Window::Create()
{
	if (EntryWindowClass() == FALSE)
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

bool Oshima::Window::EntryWindowClass()
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

void Oshima::Window::ResizeWindow(HWND window_handle)
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
