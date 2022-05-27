#pragma once

#include <Windows.h>

class MovingCircle
{
public:
	MovingCircle();
	~MovingCircle();

	void Draw(HDC hdc);

public://getter,setter
	POINT GetPos()	const { return Pos_; }
	POINT GetSize() const { return Size_; }
	void SetAllParas(const MovingCircle& mc);
	void SetPos(const POINT& p) { Pos_ = p; }
	void SetSize(const POINT& s) { Size_ = s; }


private:
	POINT Pos_;
	POINT Size_;
};

