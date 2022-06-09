#include "HermitianCurve.h"

HermitianCurve::HermitianCurve()
	: Curve_Div_(0)
	, Cal_Pt_(nullptr)
	, Start_Pos_({ 0.0f, 0.0f })
	, Start_Vec_({ 0.0f, 0.0f })
	, End_Pos_({ 0.0f, 0.0f })
	, End_Vec_({ 0.0f, 0.0f })
{
};

HermitianCurve::~HermitianCurve()
{
	if(Cal_Pt_) delete[] Cal_Pt_;
}

void HermitianCurve::Create(HermitianCurve ** out)
{
	*out = new HermitianCurve();
}

HRESULT HermitianCurve::SetParams(const UINT curve_div,
	const POINTFLOAT s_pos, const POINTFLOAT s_vec,
	const POINTFLOAT e_pos, const POINTFLOAT e_vec)
{
	if (curve_div <= 0) {
		return S_FALSE;
	}

	Curve_Div_ = curve_div;
	Cal_Pt_ = new POINTFLOAT[curve_div + 1];
	Start_Pos_ = s_pos;
	Start_Vec_ = s_vec;
	End_Pos_ = e_pos;
	End_Vec_ = e_vec;

	Calculate();

	return S_OK;
}

void HermitianCurve::Calculate()
{
	float h00, h01, h10, h11;
	float t;

	//エルミート曲線を計算
	for (UINT i = 0; i <= Curve_Div_; i++) {
		t = (float)i / Curve_Div_;				// 0<=t<=1で変化
		h00 = (t - 1)*(t - 1)*(2 * t + 1);		// エルミート補間関数H0,0
		h01 = t * t*(3 - 2 * t);				// エルミート補間関数H0,1
		h10 = (1 - t)*(1 - t)*t;					// エルミート補間関数H1,0
		h11 = (t - 1)*t*t;						// エルミート補間関数H1,1
		Cal_Pt_[i].x = h00 * Start_Pos_.x + h01 * End_Pos_.x + h10 * Start_Vec_.x + h11 * End_Vec_.x;
		Cal_Pt_[i].y = h00 * Start_Pos_.y + h01 * End_Pos_.y + h10 * Start_Vec_.y + h11 * End_Vec_.y;
	}
}
