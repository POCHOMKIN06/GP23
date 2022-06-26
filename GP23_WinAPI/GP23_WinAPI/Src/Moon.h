#pragma once

#include <Windows.h>

class Moon
{
public:
	Moon();
	Moon(const POINT& pos, FLOAT radius_out, FLOAT radius_in);
	~Moon();

	void Update(int time, const POINT& pos);
	void Draw(HDC hdc, const COLORREF& color);
private:
	POINT Pos_;
	FLOAT R_Out_;
	FLOAT R_In_;
};
