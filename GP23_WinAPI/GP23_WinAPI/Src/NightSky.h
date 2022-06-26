#pragma once

#include <Windows.h>
#include <type_traits>
//#include <DirectXColors.h>
//#include "DirectXMath.h"
#include <tchar.h>

enum class ColorsName {
	Thistle = 0,
	MediumOrchid,
	DarkViolet,
	DarkOrchid,
	Indigo,
	BlueViolet,
	MediumPurple,
	MediumSlateBlue,
	DarkSlateBlue,
	SlateBlue,
	Blue,
	MediumBlue,
	DarkBlue,
	Navy,
	MidnightBlue,
	Color_Num
};
namespace
{
	const COLORREF ColorList[]
	{
		RGB(0.847058892f *255,0.749019623f *255,0.847058892f *255),	//Thistle
		RGB(0.729411781f *255,0.333333343f *255,0.827451050f *255),	//MediumOrchid
		RGB(0.580392182f *255,0.000000000f *255,0.827451050f *255),	//DarkViolet
		RGB(0.600000024f *255,0.196078449f *255,0.800000072f *255),	//DarkOrchid
		RGB(0.294117659f *255,0.000000000f *255,0.509803951f *255),	//Indigo
		RGB(0.541176498f *255,0.168627456f *255,0.886274576f *255),	//BlueViolet
		RGB(0.576470613f *255,0.439215720f *255,0.858823597f *255),	//MediumPurple
		RGB(0.482352972f *255,0.407843173f *255,0.933333397f *255),	//MediumSlateBlue
		RGB(0.282352954f *255,0.239215702f *255,0.545098066f *255),	//DarkSlateBlue
		RGB(0.415686309f *255,0.352941185f *255,0.803921640f *255),	//SlateBlue
		RGB(0.000000000f *255,0.000000000f *255,1.000000000f *255),	//Blue
		RGB(0.000000000f *255,0.000000000f *255,0.803921640f *255),	//MediumBlue
		RGB(0.000000000f *255,0.000000000f *255,0.545098066f *255),	//DarkBlue
		RGB(0.000000000f *255,0.000000000f *255,0.501960814f *255),	//Navy
		RGB(0.098039225f *255,0.098039225f *255,0.439215720f *255)	//MidnightBlue
		//0xD8BFD8,	//Thistle
		//0xBA55D3,	//MediumOrchid
		//0x9400D3,	//DarkViolet
		//0x9932CC,	//DarkOrchid
		//0x4B0082,	//Indigo
		//0x8A2BE2,	//BlueViolet
		//0x9370DB,	//MediumPurple
		//0x7B68EE,	//MediumSlateBlue
		//0x483D8B,	//DarkSlateBlue
		//0x6A5ACD,	//SlateBlue
		//0x0000FF,	//Blue
		//0x0000CD,	//MediumBlue
		//0x00008B,	//DarkBlue
		//0x000080,	//Navy
		//0x191970	//MidnightBlue
	};
}
//operator
//前方インクリメント
inline ColorsName& operator ++ (ColorsName& c)
{
	if (static_cast<unsigned int>(c) >= (static_cast<unsigned int>(ColorsName::Color_Num)) ) {
		return c;
	}

	c = ColorsName(static_cast<std::underlying_type<ColorsName>::type>(c) + 1);
	return c;
}
inline ColorsName& operator -- (ColorsName& c)
{
	if (static_cast<unsigned int>(c) <= 0) {
		return c;
	}

	c = ColorsName(static_cast<std::underlying_type<ColorsName>::type>(c) - 1);
	return c;
}
//後方インクメント
inline ColorsName operator ++ (ColorsName& c, int)
{
	ColorsName result = c;
	++result;

	return result;
}
inline ColorsName operator -- (ColorsName& c, int)
{
	ColorsName result = c;
	--result;

	return result;
}


class NightSky
{
public:
	NightSky();
	~NightSky();

	void Update(int time);
	void Draw(HWND hWnd, HDC hdc);

	COLORREF GetColor() const { return ColorList[static_cast<int>(Sky_Color_)]; }
	const _TCHAR* GetColorName() const;
private:
	ColorsName Sky_Color_;
	HBRUSH Sky_Brush_;
	_TCHAR str[MAXCHAR];
};
