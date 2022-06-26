#include "NightSky.h"
#include <string>
#include <iostream>
NightSky::NightSky()
	: Sky_Color_(ColorsName::Thistle)
{
	Sky_Brush_ = CreateSolidBrush(ColorList[static_cast<int>(Sky_Color_)]);
}

NightSky::~NightSky()
{
	DeleteObject(Sky_Brush_);
}

void NightSky::Update(int time)
{
#ifdef _DEBUG
	int dif = 10;
#else
	int dif = 100;
#endif // _DEBUG

	if (time % dif == 0) {
		++Sky_Color_;
		if (Sky_Color_ == ColorsName::Color_Num) {
			Sky_Color_ = ColorsName::Thistle;
		}
		DeleteObject(Sky_Brush_);
		Sky_Brush_ = CreateSolidBrush(ColorList[static_cast<int>(Sky_Color_)]);
	}
	if (Sky_Color_ == ColorsName::Color_Num) {
		Sky_Color_ = ColorsName::Thistle;
	}
}

void NightSky::Draw(HWND hWnd, HDC hdc)
{
	RECT wrect;
	GetClientRect(hWnd, &wrect);
	FillRect(hdc, &wrect, Sky_Brush_);
}

const _TCHAR * NightSky::GetColorName() const
{
	switch (Sky_Color_) {
	case ColorsName::Thistle:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("Thistle"));
		return  str;
		break;
	case ColorsName::MediumOrchid:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("MediumOrchid"));
		return  str;
		break;
	case ColorsName::DarkViolet:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("DarkViolet"));
		return  str;
		break;
	case ColorsName::DarkOrchid:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("DarkOrchid"));
		return  str;
		break;
	case ColorsName::Indigo:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("Indigo"));
		return  str;
		break;
	case ColorsName::BlueViolet:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("BlueViolet"));
		return  str;
		break;
	case ColorsName::MediumPurple:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("MediumPurple"));
		return  str;
		break;
	case ColorsName::MediumSlateBlue:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("MediumSlateBlue"));
		return  str;
		break;
	case ColorsName::DarkSlateBlue:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("DarkSlateBlue"));
		return  str;
		break;
	case ColorsName::SlateBlue:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("SlateBlue"));
		return  str;
		break;
	case ColorsName::Blue:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("Blue"));
		return  str;
		break;
	case ColorsName::MediumBlue:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("MediumBlue"));
		return  str;
		break;
	case ColorsName::DarkBlue:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("DarkBlue"));
		return  str;
		break;
	case ColorsName::Navy:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("Navy"));
		return  str;
		break;
	case ColorsName::MidnightBlue:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("MidnightBlue"));
		return  str;
		break;
	case ColorsName::Color_Num:
		wsprintf(const_cast<_TCHAR*>(str), TEXT("END_COLOR"));
		return  str;
		break;
	default:
		return nullptr;
		break;
	}
}
