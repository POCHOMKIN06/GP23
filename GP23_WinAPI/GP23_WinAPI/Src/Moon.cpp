#include "Moon.h"

Moon::Moon() : Moon({100, 100}, 25, 28)
{
}

Moon::Moon(const POINT& pos, FLOAT radius_out, FLOAT radius_in)
{
	Pos_ = pos;
	R_Out_ = radius_out;
	R_In_ = radius_in;
}

Moon::~Moon()
{
}

void Moon::Update(int time, const POINT& pos)
{
	Pos_ = pos;
}

void Moon::Draw(HDC hdc, const COLORREF & color)
{
	HPEN oldP = (HPEN)SelectObject(hdc, CreatePen(PS_NULL, NULL, NULL));
	HBRUSH oldB = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
	::Ellipse(hdc, (INT)(Pos_.x - R_Out_), (INT)(Pos_.y - R_Out_), (INT)(Pos_.x + R_Out_), (INT)(Pos_.y + R_Out_));
	
	POINT pos_in = { Pos_.x - 15, Pos_.y - 20 };
	DeleteObject(SelectObject(hdc, CreateSolidBrush(color)));
	::Ellipse(hdc, (INT)(pos_in.x - R_In_), (INT)(pos_in.y - R_In_), (INT)(pos_in.x + R_In_), (INT)(pos_in.y + R_In_));
	DeleteObject(SelectObject(hdc, oldB));
	DeleteObject(SelectObject(hdc, oldP));
}
