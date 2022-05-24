#pragma once

#include <Windows.h>

class AxisLine
{
public:
	AxisLine(int x, int y);
	~AxisLine();

	void Draw(HDC hdc);
public://getter
	POINT GetOrigin() const { return Axis_; }
private:
	char str[256];
	POINT Axis_;
};

