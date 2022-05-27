#pragma once

#include <Windows.h>
#include <tchar.h>

class AxisLine
{
public:
	AxisLine(int x, int y);
	~AxisLine();

	void Draw(HDC hdc);
public://getter
	POINT GetOrigin() const { return Axis_; }
private:
	TCHAR str[256];
	POINT Axis_;
};

