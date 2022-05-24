#pragma once

#include <Windows.h>

#include "MoveType.h"
class Circle
{
public:
	Circle();
	~Circle();

	void Draw(HDC hdc);

public://getter,setter
	const POINT& GetPos()	const { return Pos_; }
	const POINT& GetSize() const { return Size_; }
	void SetPos(const POINT& p) { Pos_ = p; }
	void SetSize(const POINT& s) { Size_ = s; }

private:
	POINT Pos_;
	POINT Size_;
};


class MovingCircle
{
public:
	MovingCircle(POINT pos, POINT size);
	~MovingCircle();

	void Draw(HDC hdc);
	void Update(float time);
	void ChangeMoveType(MOVE_TYPE type);

public://getter,setter
	//getter
	const class Circle& GetCircle() const { return Circle_; }
	const enum class MOVE_TYPE& GetType() { return type_; }
	const float GetInitVel() const { return initvel; }
	const float GetAcc() const { return acc; }
	const float GetGAcc() const { return gravityAcc; }
	const float& GetTheta() { return theta; }
	//setter
	void SetTheta(float th) { theta = th; }
private:
	class Circle Circle_;
	enum class MOVE_TYPE type_;
	POINT initpos;
	float initvel;
	float acc;
	float gravityAcc;
	float theta;
};

