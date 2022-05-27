#include "AxisLine.h"

#include "main.h"

AxisLine::AxisLine(int x, int y)
	: Axis_(POINT{ x, y })
{
}

AxisLine::~AxisLine()
{
}

void AxisLine::Draw(HDC hdc)
{
	//テキスト表示
	SetTextColor(hdc, BLACK);		//テキスト色の設定
	wsprintf(str, TEXT("y"));	//バッファに格納
	TextOut(hdc, Axis_.x-10, 10, str, _tcslen(str));	//表示座標,文字列と長さ

	wsprintf(str, TEXT("x"));
	TextOut(hdc, SCREEN_WIDTH - 40, SCREEN_HEIGHT / 2 + 5, str, _tcslen(str));

	//軸表示
	//y
	MoveToEx(hdc, Axis_.x, 0, NULL);
	LineTo(hdc, Axis_.x, SCREEN_HEIGHT);
	MoveToEx(hdc, Axis_.x - 5, 10, NULL);
	LineTo(hdc, Axis_.x, 0);
	LineTo(hdc, Axis_.x + 5, 10);

	//x
	MoveToEx(hdc, 0, Axis_.y, NULL);
	LineTo(hdc, SCREEN_WIDTH, Axis_.y);
	MoveToEx(hdc, SCREEN_WIDTH - 30, Axis_.y - 5, NULL);
	LineTo(hdc, SCREEN_WIDTH - 15, Axis_.y);
	LineTo(hdc, SCREEN_WIDTH - 30, Axis_.y + 5);
}
