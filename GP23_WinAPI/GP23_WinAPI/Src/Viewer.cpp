#include "VIewer.h"

#include "main.h"
#include <cstdio>

#define ACTIVE		RGB(0,0,0)
#define DEACTIVE	RGB(192,192,192)


Viewer::Viewer()
	: Time_(0.0f)
	, InitVelecity_(0.0f)
	, Acceleration_(0.0f)
	, Gravity_(0.0f)
{
}

Viewer::~Viewer()
{
}

void Viewer::Draw(HDC hdc, MOVE_TYPE mt)
{
	int x = 25;
	int y = 50;
	int y_ofst = 20;
	int x_ofst = 120;

	SetTextColor(hdc, BLACK);

	//各パラメータ表示
	wsprintf( str, TEXT("時間経過") );
	TextOut( hdc, x, y, str, _tcslen(str) );
	_stprintf_s(str, TEXT(": %3.2f"), Time_);
	//snprintf( str, sizeof(str), TEXT(": %3.2f"), Time_);
	TextOut( hdc, x_ofst, y, str, _tcslen(str));

	wsprintf( str, TEXT("初速") );
	TextOut( hdc, x, y+y_ofst, str, _tcslen(str) );
	_stprintf_s(str, TEXT(": %3.2f"), InitVelecity_);
	//snprintf( str, sizeof(str), ": %3.2f", InitVelecity_ );
	TextOut( hdc, x_ofst, y+y_ofst, str, _tcslen(str) );

	wsprintf( str, TEXT("加速度") );
	TextOut( hdc, x, y+y_ofst*2, str, _tcslen(str) );
	_stprintf_s(str, TEXT(": %3.2f"), Acceleration_);
	//snprintf( str, sizeof(str), ": %3.2f", Acceleration_ );
	TextOut( hdc, x_ofst, y+y_ofst*2, str, _tcslen(str) );
	
	wsprintf( str, TEXT("重力加速度") );
	TextOut( hdc, x, y + y_ofst * 3, str, _tcslen(str) );
	_stprintf_s(str, TEXT(": %3.2f"), Gravity_);
	//snprintf( str, sizeof(str), ": %3.2f", Gravity_ );
	TextOut( hdc, x_ofst, y + y_ofst * 3, str, _tcslen(str) );

	wsprintf( str, TEXT("リセット") );
	TextOut( hdc, x, y + y_ofst * 5, str, _tcslen(str) );
	_stprintf_s( str, TEXT(": SPACE") );
	TextOut( hdc, x_ofst, y + y_ofst * 5, str, _tcslen(str) );


	//運動の種類表示
	int y2 = SCREEN_HEIGHT / 2 + y;
	SetTextColor(hdc, DEACTIVE);
	wsprintf( str, TEXT("1.等速運動") );
	TextOut( hdc, x, y2 + y_ofst * 0, str, _tcslen(str));
	wsprintf( str, TEXT("2.等加速運動") );
	TextOut( hdc, x, y2 + y_ofst * 1, str, _tcslen(str));
	wsprintf( str, TEXT("3.自由落下運動") );
	TextOut( hdc, x, y2 + y_ofst * 2, str, _tcslen(str));
	wsprintf( str, TEXT("4.鉛直投げ上げ運動") );
	TextOut( hdc, x, y2 + y_ofst * 3, str, _tcslen(str));
	wsprintf( str, TEXT("5.斜方投射運動") );
	TextOut( hdc, x, y2 + y_ofst * 4, str, _tcslen(str));
	wsprintf( str, TEXT("投射角") );
	TextOut( hdc, x + 10, y2 + y_ofst *5, str, _tcslen(str) );



	int arrow = y2 + y_ofst / 2 - 2;
	switch (mt) {
	case MOVE_TYPE::Constant_Velocity:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, TEXT("1.等速運動"));
		TextOut(hdc, x, y2 + y_ofst * 0, str, _tcslen(str));
		RightArrow(hdc, x, arrow + y_ofst * 0);
		break;
	case MOVE_TYPE::Constant_Acceleration:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, TEXT("2.等加速運動"));
		TextOut(hdc, x, y2 + y_ofst * 1, str, _tcslen(str));
		RightArrow(hdc, x, arrow + y_ofst * 1);
		break;
	case MOVE_TYPE::Free_Fall:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, TEXT("3.自由落下運動"));
		TextOut(hdc, x, y2 + y_ofst * 2, str, _tcslen(str));
		RightArrow(hdc, x, arrow + y_ofst * 2);
		break;
	case MOVE_TYPE::Vertical_Throw_Up:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, TEXT("4.鉛直投げ上げ運動"));
		TextOut(hdc, x, y2 + y_ofst * 3, str, _tcslen(str));
		RightArrow(hdc, x, arrow + y_ofst * 3);
		break;
	case MOVE_TYPE::Oblique_Throw:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, TEXT("5.斜方投射運動"));
		TextOut(hdc, x, y2 + y_ofst * 4, str, _tcslen(str));
		wsprintf(str, TEXT("投射角"));
		TextOut(hdc, x + 10, y2 + y_ofst * 5, str, _tcslen(str));
		_stprintf_s(str, TEXT(": %3.1f度"), Theta_);
		//snprintf(str, sizeof(str), ": %3.1f度", Theta_);
		TextOut(hdc, x_ofst, y2 + y_ofst * 5, str, _tcslen(str));
		RightArrow(hdc, x, arrow + y_ofst * 4);
		LeftArrow(hdc, x + 60, arrow + y_ofst * 5);
		RightArrow(hdc, x + 90, arrow + y_ofst * 5);
		break;
	case MOVE_TYPE::END:
		break;
	default:
		break;
	}
}

void Viewer::RightArrow(HDC hdc, int x, int y)
{
	MoveToEx(hdc, x- 3, y, NULL);
	LineTo(hdc, x - 12, y - 6);
	LineTo(hdc, x - 12, y + 6);
	LineTo(hdc, x -  3, y);
}

void Viewer::LeftArrow(HDC hdc, int x, int y)
{
	MoveToEx(hdc, x + 3, y, NULL);
	LineTo(hdc, x + 12, y - 6);
	LineTo(hdc, x + 12, y + 6);
	LineTo(hdc, x + 3, y);
}
