#pragma once
#include <Windows.h>
#include <tchar.h>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "winmm.lib")

// マクロ定義
#define CLASS_NAME	TEXT("サンプル")
#define WINDOW_NAME	TEXT("課題05 運動してる?")

#define SCREEN_WIDTH	(1280)	// ウィンドウの幅
#define SCREEN_HEIGHT	(640)	// ウィンドウの高さ

#define FPS		(20)	//更新FPS

//カラー指定
#define BLACK	RGB(0,0,0)
#define RED		RGB(255,0,0)
#define GREEN	RGB(0,255,0)
#define BLUE	RGB(0,0,255)
