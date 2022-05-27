#include "Vector.h"



Vector::Vector()
{
	x = 0;
	y = 0;
}

Vector::Vector(int xx, int yy)
{
	x = xx;
	y = yy;
}

Vector::~Vector()
{
}

void Vector::Draw(HDC hdc, POINT origin)
{
	MoveToEx(hdc, origin.x, origin.y, NULL);
	LineTo(hdc, origin.x+x, origin.y-y);
}
