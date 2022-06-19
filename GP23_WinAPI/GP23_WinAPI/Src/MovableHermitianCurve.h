#pragma once

#include <Windows.h>
#include "HermitianCurve.h"

class MovableHermitianCurve
{
public:
	MovableHermitianCurve();
	~MovableHermitianCurve();

	BOOL Create(const UINT curve_div,
		const POINTFLOAT s_pos, const POINTFLOAT s_vec,
		const POINTFLOAT e_pos, const POINTFLOAT e_vec);
	void Draw(HDC hdc);

	BOOL CheckMousePos(const int m_x, const int m_y, int* out);
	void MovePos(const POINTFLOAT new_pos, int type);

public://getter
	RECT GetRect() const { return Rect_; }
private:
	HermitianCurve* HCurve_;
	RECT Rect_;
};