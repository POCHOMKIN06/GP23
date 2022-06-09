#include "MovableHermitianCurve.h"

#include <tchar.h>

MovableHermitianCurve::MovableHermitianCurve()
	: Rect_({ -3, -3, 3, 3 })
{
	HermitianCurve::Create(&HCurve_);
}
MovableHermitianCurve::~MovableHermitianCurve()
{
	delete HCurve_;
}

BOOL MovableHermitianCurve::Create(const UINT curve_div,
	const POINTFLOAT s_pos, const POINTFLOAT s_vec,
	const POINTFLOAT e_pos, const POINTFLOAT e_vec)
{
	HRESULT hr = HCurve_->SetParams(curve_div, s_pos, s_vec, e_pos, e_vec);
	if (FAILED(hr)) {
		FALSE;
	}

	return TRUE;
}

void MovableHermitianCurve::Draw(HDC hdc)
{
#define SC_I(param) (static_cast<int>(param))

	HermitianCurve::Params ps = HCurve_->GetParams();

	// 制御ベクターの描画
	HPEN hDefaultPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 255)));
	HBRUSH hDefalutBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
	POINTFLOAT c_vec_pos[2];
	c_vec_pos[0] = POINTFLOAT{ ps.s_pos.x + ps.s_vec.x , ps.s_pos.y + ps.s_vec.y };
	c_vec_pos[1] = POINTFLOAT{ ps.e_pos.x + ps.e_vec.x , ps.e_pos.y + ps.e_vec.y };
	// 線の描画
	MoveToEx(hdc, SC_I(ps.s_pos.x), SC_I(ps.s_pos.y), NULL);
	LineTo(hdc, SC_I(c_vec_pos[0].x), SC_I(c_vec_pos[0].y));
	MoveToEx(hdc, SC_I(ps.e_pos.x), SC_I(ps.e_pos.y), NULL);
	LineTo(hdc, SC_I(c_vec_pos[1].x), SC_I(c_vec_pos[1].y));
	// 点の描画
	for (int i = 0; i < 2; i++) {
		Rectangle(hdc,
			SC_I(c_vec_pos[i].x + Rect_.left),
			SC_I(c_vec_pos[i].y + Rect_.top),
			SC_I(c_vec_pos[i].x + Rect_.right),
			SC_I(c_vec_pos[i].y + Rect_.bottom)
			);
	}


	// エルミート曲線の描画
	DeleteObject( SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 255, 0))) );
	DeleteObject( SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0))) );
	// 線の描画
	MoveToEx(hdc, SC_I(ps.c_pt[0].x), SC_I(ps.c_pt[0].y), NULL);
	for (UINT i = 1; i <= ps.c_div; i++) {
		LineTo(hdc, SC_I(ps.c_pt[i].x), SC_I(ps.c_pt[i].y));
	}
	// 点の描画
	for (UINT i = 0; i <= ps.c_div; i++) {
		Rectangle(hdc,
			SC_I(ps.c_pt[i].x + Rect_.left),
			SC_I(ps.c_pt[i].y + Rect_.top),
			SC_I(ps.c_pt[i].x + Rect_.right),
			SC_I(ps.c_pt[i].y + Rect_.bottom)
		);
	}
	
	// 制御点座標の文字表示
	COLORREF DefaultC = SetTextColor(hdc, RGB(255, 255, 255));
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	_TCHAR str[256];
	SetBkMode(hdc, TRANSPARENT);
	// 制御点
	wsprintf(str, TEXT("Control Point[0]"));
	TextOut(hdc, 5, 5 + tm.tmHeight * 0, str, _tcslen(str));
	_stprintf_s(str, TEXT(": (%4d, %4d)"), SC_I(ps.s_pos.x), SC_I(ps.s_pos.y));
	TextOut(hdc, 110, 5 + tm.tmHeight * 0, str, _tcslen(str));
	wsprintf(str, TEXT("Control Point[1]"));
	TextOut(hdc, 5, 5 + tm.tmHeight * 1, str, _tcslen(str));
	_stprintf_s(str, TEXT(": (%4d, %4d)"), SC_I(ps.e_pos.x), SC_I(ps.e_pos.y));
	TextOut(hdc, 110, 5 + tm.tmHeight * 1, str, _tcslen(str));
	// 制御ベクター
	wsprintf(str, TEXT("Control Vector[0]"));
	TextOut(hdc, 400, 5 + tm.tmHeight * 0, str, _tcslen(str));
	_stprintf_s(str, TEXT(": (%4d, %4d)"), SC_I(ps.s_vec.x), SC_I(ps.s_vec.y));
	TextOut(hdc, 510, 5 + tm.tmHeight * 0, str, _tcslen(str));
	wsprintf(str, TEXT("Control Vector[1]"));
	TextOut(hdc, 400, 5 + tm.tmHeight * 1, str, _tcslen(str));
	_stprintf_s(str, TEXT(": (%4d, %4d)"), SC_I(ps.e_vec.x), SC_I(ps.e_vec.y));
	TextOut(hdc, 510, 5 + tm.tmHeight * 1, str, _tcslen(str));

	// 各種設定を初期値に戻す
	SetBkMode(hdc, OPAQUE);
	SetTextColor(hdc, DefaultC);

	DeleteObject(SelectObject(hdc, hDefaultPen));
	DeleteObject(SelectObject(hdc, hDefalutBrush));

#undef SC_I
}
