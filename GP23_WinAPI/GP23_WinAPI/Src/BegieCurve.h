#pragma once

#include <Windows.h>

class BegieCurve
{
public:
	BegieCurve()
		: DivideNum_(100)
	{
		CulcuPoint_ = new POINTFLOAT[DivideNum_ + 1];
		ControlPoint_[0] = ControlPoint_[1] = ControlPoint_[2] = ControlPoint_[3]
		= { 0.0f, 0.0f };
	}
	~BegieCurve()
	{
		delete[] CulcuPoint_;
	}

	void DrawControlPoint(HDC hdc);
	void Draw(HDC hdc);

	void SetControlPoint(POINTFLOAT start, POINTFLOAT mid1, POINTFLOAT mid2, POINTFLOAT end, BOOL redraw = FALSE);

private:
	UINT DivideNum_;
	POINTFLOAT ControlPoint_[4];
	POINTFLOAT* CulcuPoint_;
};

