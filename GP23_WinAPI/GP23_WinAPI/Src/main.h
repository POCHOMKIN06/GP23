#pragma once
//#define STRICT				// Windowsプログラムに対する「型保証」を強化
#include <Windows.h>
#include <tchar.h>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "winmm.lib")

// マクロ定義
#define CLASS_NAME	TEXT("サンプル")
#define WINDOW_NAME	TEXT("[ GP23 Hermite2 ]")

const UINT	SCREEN_WIDTH  = 1280;				// スクリーン＆ビットマップ幅
const UINT	SCREEN_HEIGHT = 640;				// スクリーン＆ビットマップ高さ

#define FPS		(20)	//更新FPS

//カラー指定
#define BLACK	RGB(0,0,0)
#define RED		RGB(255,0,0)
#define GREEN	RGB(0,255,0)
#define BLUE	RGB(0,0,255)
