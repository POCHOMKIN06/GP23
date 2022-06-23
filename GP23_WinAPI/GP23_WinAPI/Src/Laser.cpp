#include "Laser.h"

#include "HermitianCurve.h"

Laser::Laser()
	: Status_(STATUS::STANDBY)
	, Idx_Start_(0)
	, Idx_End_(0)
{
	HermitianCurve::Create(&HCurve_);
}

Laser::~Laser()
{
	delete HCurve_;
}

BOOL Laser::Create(const UINT curve_div,
	const POINTFLOAT s_pos, const POINTFLOAT s_vec,
	const POINTFLOAT e_pos, const POINTFLOAT e_vec)
{
	HRESULT hr = HCurve_->SetParams(curve_div, s_pos, s_vec, e_pos, e_vec);
	if (FAILED(hr)) {
		FALSE;
	}

	return TRUE;
}

void Laser::Update()
{	
	HermitianCurve::Params ps = HCurve_->GetParams();

	//描画用始点と終点を設定
	switch (Status_) {
	case Laser::STATUS::STANDBY:
		Idx_Start_ = 0;
		Idx_End_ = 0;
		break;
	case Laser::STATUS::SHOOT:
		if (static_cast<UINT>(Idx_End_) < ps.c_div) {	// レーザーを敵まで伸ばす
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
		Status_ = Laser::STATUS::STANDBY;
		break;
	default:
		break;
	}
}

void Laser::Update(const POINTFLOAT s_pos, const POINTFLOAT e_pos)
{
	HCurve_->SetPoses(s_pos, e_pos);
	Update();
}

void Laser::Draw(HDC hdc)
{
#define SC_I(param) (static_cast<int>(param))

	HPEN hDefaultPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(0, 255, 255)));	// レーザー用ペンを作成

	HermitianCurve::Params ps = HCurve_->GetParams();

	MoveToEx(hdc, SC_I(ps.c_pt[Idx_Start_].x), SC_I(ps.c_pt[Idx_Start_].y), NULL);
	for (INT i = Idx_Start_ + 1; i <= Idx_End_; i++) {
		LineTo(hdc, SC_I(ps.c_pt[i].x), SC_I(ps.c_pt[i].y));
	}

	DeleteObject(SelectObject(hdc, hDefaultPen));

#undef SC_I
}
