#include "MovingCircle.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "main.h"
#include "MoveType.h"


//ÉpÉâÉÅÅ[É^ÉvÉäÉZÉbÉg
constexpr float INIT_VEL = 10.0f;
constexpr float ACCELERATION = 5.0f;
constexpr float GRAVITY = -9.8f;

Circle::Circle()
	: Pos_(POINT{ 0,0 })
	, Size_(POINT{ 0,0 })
{
}

Circle::~Circle()
{
}

void Circle::Draw(HDC hdc)
{	
	//â~Çï`âÊ
	Ellipse(hdc,
		Pos_.x - Size_.x / 2,
		Pos_.y - Size_.y / 2,
		Pos_.x + Size_.x / 2,
		Pos_.y + Size_.y / 2
	);
}

MovingCircle::MovingCircle(POINT pos, POINT size)
	: initpos(pos)
	, type_(MOVE_TYPE::Constant_Velocity)
	, Circle_()
{
	Circle_.SetPos(pos);
	Circle_.SetSize(size);
	ChangeMoveType(type_);
}

MovingCircle::~MovingCircle()
{
}

void MovingCircle::Draw(HDC hdc)
{
	Circle_.Draw(hdc);
}

void MovingCircle::Update(float time)
{
	POINT pos = Circle_.GetPos();
	switch (type_) {
	case MOVE_TYPE::Constant_Velocity://ìôë¨â^ìÆ
		pos.x = static_cast<int>(initpos.x + initvel * time);
		pos.y = static_cast<int>(initpos.y);
		break;
	case MOVE_TYPE::Constant_Acceleration://ìôâ¡ë¨â^ìÆ
		pos.x = static_cast<int>(initpos.x + initvel * time + 0.5f*acc*time*time);
		pos.y = static_cast<int>(initpos.y);
		break;
	case MOVE_TYPE::Free_Fall://é©óRóéâ∫â^ìÆ
		pos.x = static_cast<int>(initpos.x);
		pos.y = static_cast<int>(initpos.y - 0.5f*gravityAcc*time*time);
		break;
	case MOVE_TYPE::Vertical_Throw_Up://âîíºìäÇ∞è„Ç∞â^ìÆ
		pos.x = static_cast<int>(initpos.x);
		pos.y = static_cast<int>(initpos.y - initvel * time - 0.5f*gravityAcc*time*time);
		break;
	case MOVE_TYPE::Oblique_Throw://éŒï˚ìäéÀâ^ìÆ
		pos.x = static_cast<int>(initpos.x - initvel * time * cos((-theta + 180.0f)*M_PI / 180.0f));
		pos.y = static_cast<int>(initpos.y - initvel * time * sin(theta*M_PI / 180.0f) - 0.5f*gravityAcc*time*time);
		break;
	case MOVE_TYPE::END:
		break;
	default:
		break;
	}
	Circle_.SetPos(pos);
}

void MovingCircle::ChangeMoveType(MOVE_TYPE type)
{
	type_ = type;
	switch (type_) {
	case MOVE_TYPE::Constant_Velocity:
		initvel = INIT_VEL;
		acc = 0.0f;
		gravityAcc = 0.0f;
		break;
	case MOVE_TYPE::Constant_Acceleration:
		initvel = INIT_VEL;
		acc = ACCELERATION;
		gravityAcc = 0.0f;
		break;
	case MOVE_TYPE::Free_Fall:
		initvel = 0.0f;
		acc = 0.0f;
		gravityAcc = GRAVITY;
		break;
	case MOVE_TYPE::Vertical_Throw_Up:
		initvel = 70.0f;
		acc = 0.0f;
		gravityAcc = GRAVITY;
		break;
	case MOVE_TYPE::Oblique_Throw:
		initvel = 80.0f;
		acc = 0.0f;
		gravityAcc = GRAVITY;
		theta = 30.0f;
		break;
	case MOVE_TYPE::END:
		break;
	default:
		break;
	}

}
