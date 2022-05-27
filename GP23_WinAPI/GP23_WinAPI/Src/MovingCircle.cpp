#include "MovingCircle.h"

MovingCircle::MovingCircle()
	: Pos_(POINT{ 0,0 })
	, Size_(POINT{ 0,0 })
{
}

MovingCircle::~MovingCircle()
{
}

void MovingCircle::Draw(HDC hdc)
{	
	//‰~‚ð•`‰æ
	Ellipse(hdc,
		Pos_.x - Size_.x / 2,
		Pos_.y - Size_.y / 2,
		Pos_.x + Size_.x / 2,
		Pos_.y + Size_.y / 2
	);
}

void MovingCircle::SetAllParas(const MovingCircle & mc)
{
	Pos_ = mc.GetPos();
	Size_ = mc.GetSize();
}
