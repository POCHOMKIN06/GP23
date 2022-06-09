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
	COLORREF DefaultC = SetTextColor(hdc, (COLORREF)GetStockObject(BLACK_PEN));
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

BOOL MovableHermitianCurve::CheckMousePos(const int m_x, const int m_y, int * out)
{
	typedef struct _ControlPos {
		POINTFLOAT s_pos;
		POINTFLOAT s_vec_pos;
		POINTFLOAT e_pos;
		POINTFLOAT e_vec_pos;
	}ContPos;

	HermitianCurve::Params ps = HCurve_->GetParams();
	ContPos cp = {
		ps.s_pos,
		POINTFLOAT{ ps.s_pos.x + ps.s_vec.x , ps.s_pos.y + ps.s_vec.y },
		ps.e_pos,
		POINTFLOAT{ ps.e_pos.x + ps.e_vec.x , ps.e_pos.y + ps.e_vec.y }
	};

	RECT c_rect[4];
	c_rect[0] = RECT{ (int)(cp.s_pos.x + Rect_.left),		(int)(cp.s_pos.y + Rect_.top),	   (int)(cp.s_pos.x + Rect_.right),		(int)(cp.s_pos.y + Rect_.bottom) };
	c_rect[1] = RECT{ (int)(cp.s_vec_pos.x + Rect_.left),	(int)(cp.s_vec_pos.y + Rect_.top), (int)(cp.s_vec_pos.x + Rect_.right), (int)(cp.s_vec_pos.y + Rect_.bottom) };
	c_rect[2] = RECT{ (int)(cp.e_pos.x + Rect_.left),		(int)(cp.e_pos.y + Rect_.top),	   (int)(cp.e_pos.x + Rect_.right),		(int)(cp.e_pos.y + Rect_.bottom) };
	c_rect[3] = RECT{ (int)(cp.e_vec_pos.x + Rect_.left),	(int)(cp.e_vec_pos.y + Rect_.top), (int)(cp.e_vec_pos.x + Rect_.right), (int)(cp.e_vec_pos.y + Rect_.bottom) };

	if (m_x >= c_rect[0].left && m_x <= c_rect[0].right
		&& m_y >= c_rect[0].top && m_y <= c_rect[0].bottom) {
		*out = 0;
		return TRUE;
	}
	if (m_x >= c_rect[1].left && m_x <= c_rect[1].right
		&& m_y >= c_rect[1].top && m_y <= c_rect[1].bottom) {
		*out = 1;
		return TRUE;
	}
	if (m_x >= c_rect[2].left && m_x <= c_rect[2].right
		&& m_y >= c_rect[2].top && m_y <= c_rect[2].bottom) {
		*out = 2;
		return TRUE;
	}
	if (m_x >= c_rect[3].left && m_x <= c_rect[3].right
		&& m_y >= c_rect[3].top && m_y <= c_rect[3].bottom) {
		*out = 3;
		return TRUE;
	}

	//*out = -1;
	return FALSE;
}

void MovableHermitianCurve::MovePos(const POINTFLOAT new_pos, int type)
{
	switch (type) {
	case 0:	// s_pos
		HCurve_->SetSPos(new_pos);
		break;
	case 1:	// s_vec
	{
		HermitianCurve::Params ps = HCurve_->GetParams();
		HCurve_->SetSVec(POINTFLOAT{ new_pos.x - ps.s_pos.x, new_pos.y - ps.s_pos.y });
	}
		break;
	case 2:	// e_pos
		HCurve_->SetEPos(new_pos);
		break;
	case 3:	// e_vec
	{	
		HermitianCurve::Params ps = HCurve_->GetParams();
		HCurve_->SetEVec(POINTFLOAT{ new_pos.x - ps.e_pos.x, new_pos.y - ps.e_pos.y });
	}
		break;
	default:
		break;
	}
}
