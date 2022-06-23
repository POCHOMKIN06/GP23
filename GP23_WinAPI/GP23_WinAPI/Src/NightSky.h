#pragma once

#include <Windows.h>
#include <type_traits>
//#include <DirectXColors.h>
//#include "DirectXMath.h"

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
		0xD8BFD8,	//Thistle
		0xBA55D3,	//MediumOrchid
		0x9400D3,	//DarkViolet
		0x9932CC,	//DarkOrchid
		0x4B0082,	//Indigo
		0x8A2BE2,	//BlueViolet
		0x9370DB,	//MediumPurple
		0x7B68EE,	//MediumSlateBlue
		0x483D8B,	//DarkSlateBlue
		0x6A5ACD,	//SlateBlue
		0x0000FF,	//Blue
		0x0000CD,	//MediumBlue
		0x00008B,	//DarkBlue
		0x000080,	//Navy
		0x191970	//MidnightBlue
	};
}
//operator
//前方インクリメント
inline ColorsName& operator ++ (ColorsName& c)
{
	if (static_cast<unsigned int>(c) >= static_cast<unsigned int>(ColorsName::Color_Num)) {
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

	void Update();
	void Draw(HWND hWnd, HDC hdc);

private:
	ColorsName Sky_Color_;
	HBRUSH Sky_Brush_;
};
