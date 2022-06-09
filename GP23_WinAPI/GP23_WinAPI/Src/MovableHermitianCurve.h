#pragma once

#include <Windows.h>
#include "HermitianCurve.h"

class MovableHermitianCurve
{
public:
	typedef struct _ControlPos {
		POINTFLOAT s_pos;
		POINTFLOAT s_vec_pos;
		POINTFLOAT e_pos;
		POINTFLOAT e_vec_pos;
	}ContPos;
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
	const RECT& GetRect() const { return Rect_; }
	const ContPos GetControlPos() const
	{
		HermitianCurve::Params ps = HCurve_->GetParams();
		ContPos cp = {
			ps.s_pos,
			POINTFLOAT{ ps.s_pos.x + ps.s_vec.x , ps.s_pos.y + ps.s_vec.y },
			ps.e_pos,
			POINTFLOAT{ ps.e_pos.x + ps.e_vec.x , ps.e_pos.y + ps.e_vec.y }
		};
		return cp;
	}
private:
	HermitianCurve* HCurve_;
	RECT Rect_;
};