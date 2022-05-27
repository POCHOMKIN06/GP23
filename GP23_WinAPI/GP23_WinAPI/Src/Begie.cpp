#include "Begie.h"

void Begie::Draw(HDC hdc)
{	
	//§Œä“_‚Ì•`‰æ
	MoveToEx(hdc, static_cast<int>(Pos_[0].x), static_cast<int>(Pos_[0].y), NULL);
	for (int i = 0; i < sizeof(Pos_) / sizeof(POINTFLOAT); i++) {
		LineTo(hdc, static_cast<int>(Pos_[i].x), static_cast<int>(Pos_[i].y));
	}

	//‹Èü‚ğŒvZ
	MoveToEx(hdc, static_cast<int>(Pos_[0].x), static_cast<int>(Pos_[0].y), NULL);
	float t = 0.0f;
	for (unsigned int i = 0; i <= Devide_; i++) {
		DevPoint_[i].x =
			(1.0f - t)*(1.0f - t)*(1.0f - t)*Pos_[0].x
			+ 3 * (1.0f - t)*(1.0f - t)*t*Pos_[1].x
			+ 3 * (1.0f - t)*t*t*Pos_[2].x
			+ t * t*t*Pos_[3].x;
		DevPoint_[i].y =
			(1.0f - t)*(1.0f - t)*(1.0f - t)*Pos_[0].y
			+ 3 * (1.0f - t)*(1.0f - t)*t*Pos_[1].y
			+ 3 * (1.0f - t)*t*t*Pos_[2].y
			+ t * t*t*Pos_[3].y;
		t += 1.0f / Devide_;
	}

	for (unsigned int i = 0; i <= Devide_; i++) {
		LineTo(hdc, static_cast<int>(DevPoint_[i].x), static_cast<int>(DevPoint_[i].y));
	}
}
