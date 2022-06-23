#include "NightSky.h"

NightSky::NightSky()
	: Sky_Color_(ColorsName::MidnightBlue)
{
	Sky_Brush_ = CreateSolidBrush(ColorList[static_cast<int>(Sky_Color_)]);
}

NightSky::~NightSky()
{
	DeleteObject(Sky_Brush_);
}

void NightSky::Update()
{
	static int cnt = 0;
	//cnt++;
	//if (cnt % 3 == 0) {
	//	++Sky_Color_;
	//	DeleteObject(Sky_Brush_);
	//	Sky_Brush_ = CreateSolidBrush(ColorList[static_cast<COLORREF>(Sky_Color_)]);
	//	cnt = 0;
	//}
}

void NightSky::Draw(HWND hWnd, HDC hdc)
{
	RECT wrect;
	GetClientRect(hWnd, &wrect);
	FillRect(hdc, &wrect, Sky_Brush_);

}
