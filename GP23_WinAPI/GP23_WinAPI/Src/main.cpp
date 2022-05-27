//課題05　運動してる?
//2:投下速度運動、3:自由落下運動、5:斜方投射を画面内に表現する
//提出期限:5/13(金)

// ヘッダー
#include "main.h"

#include <iostream>
#include <cstdio>
#include "strsafe.h"


#include "AxisLine.h"
#include "VIewer.h"
#include "MoveType.h"
#include "MovingCircle.h"

//マクロ定義
#define IDT_TIMER1	(100)

// プロトタイプ宣言
LRESULT	CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(void);
void TimeReset(void);
void UpdateMoveType(void);

//パラメータプリセット
constexpr float INIT_VEL     = 10.0f;
constexpr float ACCELERATION = 4.0f;
constexpr float GRAVITY      = -9.8f;

// グローバル変数
HINSTANCE g_hInstance;
float g_time;
float g_initvel;
float g_acc;
float g_gravityAcc;
float g_theta;
POINT g_origin = { 200, SCREEN_HEIGHT / 2 };

//自作オブジェクト
AxisLine		g_AxisLine(g_origin.x, g_origin.y);		//座標軸
Viewer			g_Viewer;								//各種パラメータ表示
MOVE_TYPE		g_MoveType = MOVE_TYPE::Constant_Velocity;				//運動挙動の種類
MovingCircle	g_Circle[static_cast<unsigned int>(MOVE_TYPE::END)];	//運動する円



// main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	g_hInstance = hInstance;

	Init();

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
	//char str[256];		//文字列表示用
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
		switch (wParam)
		{
		case IDT_TIMER1:
			//データコピー
			MovingCircle* temp = &g_Circle[static_cast<unsigned int>(g_MoveType)];

			//時間の経過
			g_time += 0.5f;
			g_Viewer.SetTime(g_time);

			//時間リセット
			POINT pos = temp->GetPos();
			if (pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y > SCREEN_HEIGHT || pos.y < 0) {
				TimeReset();
			}

			switch (g_MoveType)
			{
			case MOVE_TYPE::Constant_Velocity://等速運動
				pos.x = static_cast<int>(g_origin.x + g_initvel * g_time);
				pos.y = static_cast<int>(g_origin.y);
				break;
			case MOVE_TYPE::Constant_Acceleration://等加速運動
				pos.x = static_cast<int>(g_origin.x + g_initvel * g_time + 0.5f*g_acc*g_time*g_time);
				pos.y = static_cast<int>(g_origin.y);
				break;
			case MOVE_TYPE::Free_Fall://自由落下運動
				pos.x = static_cast<int>(g_origin.x);
				pos.y = static_cast<int>(g_origin.y - 0.5f*g_gravityAcc*g_time*g_time);
				break;
			case MOVE_TYPE::Vertical_Throw_Up://鉛直投げ上げ運動
				pos.x = static_cast<int>(g_origin.x);
				pos.y = static_cast<int>(g_origin.y - g_initvel * g_time - 0.5f*g_gravityAcc*g_time*g_time);
				break;
			case MOVE_TYPE::Oblique_Throw://斜方投射運動
				pos.x = static_cast<int>(g_origin.x - g_initvel * g_time * cos((-g_theta + 180.0f)*M_PI / 180.0f));
				pos.y = static_cast<int>(g_origin.y - g_initvel * g_time * sin(g_theta*M_PI / 180.0f) - 0.5f*g_gravityAcc*g_time*g_time);
				break;
			case MOVE_TYPE::END:
				break;
			default:
				break;
			}
			temp->SetPos(pos);


			//再描画を指示
			RECT rect = {
				0,0, SCREEN_WIDTH, SCREEN_HEIGHT
			};
			InvalidateRect(hWnd, &rect, TRUE);

			break;
		//default:
		//	break;
		}
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
		g_Viewer.Draw(hDC, g_MoveType);

		//ペンを作成
		hPen = CreatePen(PS_SOLID, 3, GREEN);	//線種,太さ,色
		//ペンを持ち替え
		hPenOld = (HPEN)SelectObject(hDC, hPen);

		//円を描画
		g_Circle[static_cast<unsigned int>(g_MoveType)].Draw(hDC);

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
			--g_MoveType;
			TimeReset();
			UpdateMoveType();
			break;
		case VK_DOWN:
			++g_MoveType;
			TimeReset();
			UpdateMoveType();
			break;
		case VK_RIGHT:
			if (g_MoveType == MOVE_TYPE::Oblique_Throw) {
				g_theta -= 5.0f;
				if (g_theta < 0.0f) {
					g_theta = 355.0f;
				}
				g_Viewer.SetTheta(g_theta);
				TimeReset();
			}
			break;
		case VK_LEFT:
			if (g_MoveType == MOVE_TYPE::Oblique_Throw) {
				g_theta += 5.0f;
				if (g_theta > 355.0f) {
					g_theta = 0.0;
				}
				g_Viewer.SetTheta(g_theta);
				TimeReset();
			}
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
			TimeReset();
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

void Init(void)
{
	for (auto&& c : g_Circle) {
		c.SetPos(g_origin);
		c.SetSize({ 15, 15 });
	}

	g_time       = 0.0f;
	g_initvel    = INIT_VEL;
	g_acc        = 0.0f;
	g_gravityAcc = 0.0f;
	g_theta      = 30.0f;

	g_Viewer.SetTime(g_time);
	g_Viewer.SetInitVel(g_initvel);
	g_Viewer.SetAcc(g_acc);
	g_Viewer.SetGravity(g_gravityAcc);
}

void TimeReset(void)
{
	g_time = 0.0f;
}

void UpdateMoveType(void)
{
	switch (g_MoveType) {
	case MOVE_TYPE::Constant_Velocity:
		g_initvel = INIT_VEL;
		g_acc = 0.0f;
		g_gravityAcc = 0.0f;
		break;
	case MOVE_TYPE::Constant_Acceleration:
		g_initvel = INIT_VEL;
		g_acc = ACCELERATION;
		g_gravityAcc = 0.0f;
		break;
	case MOVE_TYPE::Free_Fall:
		g_initvel = 0.0f;
		g_acc = 0.0f;
		g_gravityAcc = GRAVITY;
		break;
	case MOVE_TYPE::Vertical_Throw_Up:
		g_initvel = 70.0f;
		g_acc = 0.0f;
		g_gravityAcc = GRAVITY;
		break;
	case MOVE_TYPE::Oblique_Throw:
		g_initvel = 80.0f;
		g_acc = 0.0f;
		g_gravityAcc = GRAVITY;
		break;
	case MOVE_TYPE::END:
		break;
	default:
		break;
	}
	g_Viewer.SetInitVel(g_initvel);
	g_Viewer.SetAcc(g_acc);
	g_Viewer.SetGravity(g_gravityAcc);
	g_Viewer.SetTheta(g_theta);
}
