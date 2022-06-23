#include "Fireworks.h"

#include "HermitianCurve.h"
#include "main.h"


const POINTFLOAT Fireworks::defSPos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100 };
const POINTFLOAT Fireworks::defEPos = { SCREEN_WIDTH / 2, 100 };
const POINTFLOAT Fireworks::defSVec = { 0, -500 };
const POINTFLOAT Fireworks::defEVec = { 0, -500 };


Fireworks::Fireworks()
{
	POINTFLOAT s_pos = { rand() % (SCREEN_WIDTH / 2) + defSPos.x, defSPos.y };
	POINTFLOAT e_pos = { rand() % (SCREEN_WIDTH / 4) + defEPos.x, rand() % (SCREEN_HEIGHT / 4) + defEPos.y };
	POINTFLOAT s_vec = { rand() % (SCREEN_WIDTH / 3) + defSVec.x, rand() % (SCREEN_HEIGHT / 10) + defSVec.y };
	POINTFLOAT e_vec = { rand() % (SCREEN_WIDTH / 3) + defEVec.x, rand() % (SCREEN_HEIGHT / 10) + defEVec.y };

	Laser::Create(30, s_pos, s_vec, e_pos, e_vec);
}

Fireworks::~Fireworks()
{
	
}

void Fireworks::Update()
{
	//Laser::Update()の一部を改変
	HermitianCurve::Params ps = HCurve_->GetParams();

	//描画用始点と終点を設定
	switch (Status_) {
	case Laser::STATUS::STANDBY:
		Idx_Start_ = 0;
		Idx_End_ = 0;


		break;
	case Laser::STATUS::SHOOT:
		if (static_cast<UINT>(Idx_End_) < ps.c_div) {
			Idx_End_++;
			Idx_Start_ = Idx_End_ - 3;
			if (Idx_Start_ <= 0) {
				Idx_Start_ = 0;
			}
		} else if (static_cast<UINT>(Idx_Start_) < ps.c_div) {
			Idx_Start_++;
		} else {	// 次へ
			Status_ = STATUS::END;
		}
		break;
	case Laser::STATUS::END:
		//
		//ここでパーティクル生成
		//
		Status_ = Laser::STATUS::STANDBY;
		break;
	default:
		break;
	}
}

void Fireworks::Update(const POINTFLOAT s_pos, const POINTFLOAT e_pos)
{
	Laser::Update(s_pos, e_pos);
}

void Fireworks::Draw(HDC hdc)
{
#define SC_I(param) (static_cast<int>(param))

	HPEN hDefaultPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 255, 255)));	// レーザー用ペンを作成

	HermitianCurve::Params ps = HCurve_->GetParams();

	MoveToEx(hdc, SC_I(ps.c_pt[Idx_Start_].x), SC_I(ps.c_pt[Idx_Start_].y), NULL);
	for (INT i = Idx_Start_ + 1; i <= Idx_End_; i++) {
		if (i % 31 == 0 || i % 4 == 0) {
			DeleteObject(SelectObject(hdc, CreatePen(PS_SOLID, 4, RGB(0, 255, 255))));
		} else {
			DeleteObject(SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 255, 255))));
		}
		LineTo(hdc, SC_I(ps.c_pt[i].x), SC_I(ps.c_pt[i].y));
	}

	DeleteObject(SelectObject(hdc, hDefaultPen));

#undef SC_I
}
