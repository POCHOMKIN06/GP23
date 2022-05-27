#pragma once

#include <Windows.h>
#include <tchar.h>

#include "MoveType.h"

class Viewer
{
public:
	Viewer();
	~Viewer();

	void Draw(HDC hdc, MOVE_TYPE mt);
public://setter
	void SetTime(const float& t) { Time_ = t; }
	void SetInitVel(const float& v) { InitVelecity_ = v; }
	void SetAcc(const float& a) { Acceleration_ = a; }
	void SetGravity(const float& g) { Gravity_ = g; }
	void SetTheta(const float& t) { Theta_ = t; }
private:
	void RightArrow(HDC hdc, int x, int y);
	void LeftArrow(HDC hdc, int x, int y);
private:
	TCHAR str[256];
	float Time_;
	float InitVelecity_;
	float Acceleration_;
	float Gravity_;
	float Theta_;
};
