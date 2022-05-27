#pragma once

#include <Windows.h>

class Vector : private POINT
{
public:
	Vector();
	Vector(int, int);
	~Vector();

	void Draw(HDC, POINT);
private:
};
