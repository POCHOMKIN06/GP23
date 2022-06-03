#include "BegieCurve.h"

#include "Window.h"
#include "main.h"

void BegieCurve::DrawControlPoint(HDC hdc)
{
	//êßå‰ì_ÇÃï`âÊ
	MoveToEx(hdc, static_cast<int>(ControlPoint_[0].x), static_cast<int>(ControlPoint_[0].y), NULL);
	for (int i = 0; i < sizeof(ControlPoint_) / sizeof(POINTFLOAT); i++) {
		LineTo(hdc, static_cast<int>(ControlPoint_[i].x), static_cast<int>(ControlPoint_[i].y));
	}
}

void BegieCurve::Draw(HDC hdc)
{	
	//ã»ê¸ÇåvéZ
	MoveToEx(hdc, static_cast<int>(ControlPoint_[0].x), static_cast<int>(ControlPoint_[0].y), NULL);
	float t = 0.0f;
	for (UINT i = 0; i <= DevideNum_; i++) {
		CulcuPoint_[i].x =
			(1.0f - t)*(1.0f - t)*(1.0f - t)*ControlPoint_[0].x
			+ 3 * (1.0f - t)*(1.0f - t)*t*ControlPoint_[1].x
			+ 3 * (1.0f - t)*t*t*ControlPoint_[2].x
			+ t * t*t*ControlPoint_[3].x;
		CulcuPoint_[i].y =
			(1.0f - t)*(1.0f - t)*(1.0f - t)*ControlPoint_[0].y
			+ 3 * (1.0f - t)*(1.0f - t)*t*ControlPoint_[1].y
			+ 3 * (1.0f - t)*t*t*ControlPoint_[2].y
			+ t * t*t*ControlPoint_[3].y;
		t += 1.0f / DevideNum_;
	}

	for (UINT i = 0; i <= DevideNum_; i++) {
		LineTo(hdc, static_cast<int>(CulcuPoint_[i].x), static_cast<int>(CulcuPoint_[i].y));
	}
}

void BegieCurve::SetControlPoint(POINTFLOAT start, POINTFLOAT mid1, POINTFLOAT mid2, POINTFLOAT end, BOOL redraw)
{
	ControlPoint_[0] = start;
	ControlPoint_[1] = mid1;
	ControlPoint_[2] = mid2;
	ControlPoint_[3] = end;
	
	if (redraw) {
		HWND hWnd =  FindWindow(Window::ClassName, WINDOW_NAME);
		RECT rect;
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, &rect, TRUE);
	}
}
