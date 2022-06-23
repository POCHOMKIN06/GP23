#include "StarLight.h"

StarLight::StarLight(POINT pos, SIZE size)
	: size_(size)
	, pos_(pos)
{
	Init(size);
}

StarLight::~StarLight()
{
}

void StarLight::Update()
{
	static int cnt = 0;
	cnt++;
	if (cnt % 5 == 0) {
		radius_++;
		switch (size_) {
		case StarLight::SIZE::Small:
			if (radius_ >= 2) {
				radius_ = 2;
			}
			break;
		case StarLight::SIZE::Middle:
			if (radius_ >= 3) {
				radius_ = 3;
			}
			break;
		case StarLight::SIZE::Large:
			if (radius_ >= 4) {
				radius_ = 4;
			}
			break;
		default:
			break;
		}
	} 
	if (cnt % 11 == 0) {
		radius_--;
		if (radius_ <= 0) {
			radius_ = 0;
		}
	}
}

void StarLight::Draw(HDC hdc)
{
	HPEN hOldP = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(255, 255, 255)));
	HBRUSH hOldB = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
	int oldmode = SetPolyFillMode(hdc, ALTERNATE);//WINDING:Œğ·ƒGƒŠƒA‚ğ“h‚è‚Â‚Ô‚µ,ALTERNATE:“h‚è‚Â‚Ô‚³‚È‚¢
	::Ellipse(hdc, pos_.x - radius_, pos_.y - radius_, pos_.x + radius_, pos_.y + radius_);
	::Polygon(hdc, poly_pos_, 8);
	
	DeleteObject(hOldP);
	DeleteObject(hOldB);
	SetPolyFillMode(hdc, oldmode);
}

void StarLight::Init(SIZE size)
{
	switch (size) {
	case StarLight::SIZE::Small:
		radius_ = 1;
		break;
	case StarLight::SIZE::Middle:
		radius_ = 2;
		break;
	case StarLight::SIZE::Large:
		radius_ = 3;
		break;
	default:
		break;
	}
	int n = radius_ + 2;
	int m = radius_ / 2;
	poly_pos_[0].x = pos_.x;		poly_pos_[0].y = pos_.y - n;
	poly_pos_[1].x = pos_.x - m;	poly_pos_[1].y = pos_.y - m;
	poly_pos_[2].x = pos_.x - n;	poly_pos_[2].y = pos_.y;
	poly_pos_[3].x = pos_.x - m;	poly_pos_[3].y = pos_.y + m;
	poly_pos_[4].x = pos_.x;		poly_pos_[4].y = pos_.y + n;
	poly_pos_[5].x = pos_.x + m;	poly_pos_[5].y = pos_.y + m;
	poly_pos_[6].x = pos_.x + n;	poly_pos_[6].y = pos_.y;
	poly_pos_[7].x = pos_.x + m;	poly_pos_[7].y = pos_.y - m;


}
