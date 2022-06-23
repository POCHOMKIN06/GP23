#pragma once

#include "Laser.h"

class Fireworks : public Laser
{
public:
	Fireworks();
	~Fireworks();

	void Update();
	void Update(const POINTFLOAT s_pos, const POINTFLOAT e_pos);

	void Draw(HDC hdc);

private:
	static const POINTFLOAT defSPos;
	static const POINTFLOAT defEPos;
	static const POINTFLOAT defSVec;
	static const POINTFLOAT defEVec;

	//Particle* Explosion_;
	//Particle* Spark_;
};

//s_posとe_posをランダムで決定
//線の太さを太くしたり細くしたりする	**
//STATUS::ENDでパーティクル生成
//爆発、火花の2種類を作る
//観客席を作る
//背景を夜にする
//星を作る
