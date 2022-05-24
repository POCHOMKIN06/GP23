#include "VIewer.h"

#include "main.h"
#include <cstdio>
#include "MoveType.h"

#define ACTIVE		RGB(0,0,0)
#define DEACTIVE	RGB(192,192,192)


Viewer::Viewer()
	: Time_(0.0f)
	, InitVelecity_(0.0f)
	, Acceleration_(0.0f)
	, Gravity_(0.0f)
{
}

Viewer::~Viewer()
{
}

void Viewer::Draw(HDC hdc, MOVE_TYPE mt)
{
	int x = 25;
	int y = 50;
	int y_ofst = 20;
	int x_ofst = 120;

	SetTextColor(hdc, BLACK);

	//Šeƒpƒ‰ƒ[ƒ^•\¦
	wsprintf(str, "ŠÔŒo‰ß");
	TextOut(hdc, x, y, str, strlen(str));
	snprintf(str, sizeof(str), ": %3.2f", Time_);
	TextOut(hdc, x_ofst, y, str, strlen(str));

	wsprintf(str, "‰‘¬");
	TextOut(hdc, x, y+y_ofst, str, strlen(str));
	snprintf(str, sizeof(str), ": %3.2f", InitVelecity_);
	TextOut(hdc, x_ofst, y+y_ofst, str, strlen(str));

	wsprintf(str, "‰Á‘¬“x");
	TextOut(hdc, x, y+y_ofst*2, str, strlen(str));
	snprintf(str, sizeof(str), ": %3.2f", Acceleration_);
	TextOut(hdc, x_ofst, y+y_ofst*2, str, strlen(str));
	
	wsprintf(str, "d—Í‰Á‘¬“x");
	TextOut(hdc, x, y + y_ofst * 3, str, strlen(str));
	snprintf(str, sizeof(str), ": %3.2f", Gravity_);
	TextOut(hdc, x_ofst, y + y_ofst * 3, str, strlen(str));

	wsprintf(str, "ƒŠƒZƒbƒg");
	TextOut(hdc, x, y + y_ofst * 5, str, strlen(str));
	wsprintf(str, ": SPACE");
	TextOut(hdc, x_ofst, y + y_ofst * 5, str, strlen(str));


	//‰^“®‚Ìí—Ş•\¦
	int y2 = SCREEN_HEIGHT / 2 + y;
	SetTextColor(hdc, DEACTIVE);
	wsprintf(str, "1.“™‘¬‰^“®");
	TextOut(hdc, x, y2 + y_ofst * 0, str, strlen(str));
	wsprintf(str, "2.“™‰Á‘¬‰^“®");
	TextOut(hdc, x, y2 + y_ofst * 1, str, strlen(str));
	wsprintf(str, "3.©—R—‰º‰^“®");
	TextOut(hdc, x, y2 + y_ofst * 2, str, strlen(str));
	wsprintf(str, "4.‰”’¼“Š‚°ã‚°‰^“®");
	TextOut(hdc, x, y2 + y_ofst * 3, str, strlen(str));
	wsprintf(str, "5.Î•û“ŠË‰^“®");
	TextOut(hdc, x, y2 + y_ofst * 4, str, strlen(str));
	wsprintf(str, "“ŠËŠp");
	TextOut(hdc, x + 10, y2 + y_ofst *5, str, strlen(str));



	int arrow = y2 + y_ofst / 2 - 2;
	switch (mt) {
	case MOVE_TYPE::Constant_Velocity:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, "1.“™‘¬‰^“®");
		TextOut(hdc, x, y2 + y_ofst * 0, str, strlen(str));
		RightArrow(hdc, x, arrow + y_ofst * 0);
		break;
	case MOVE_TYPE::Constant_Acceleration:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, "2.“™‰Á‘¬‰^“®");
		TextOut(hdc, x, y2 + y_ofst * 1, str, strlen(str));
		RightArrow(hdc, x, arrow + y_ofst * 1);
		break;
	case MOVE_TYPE::Free_Fall:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, "3.©—R—‰º‰^“®");
		TextOut(hdc, x, y2 + y_ofst * 2, str, strlen(str));
		RightArrow(hdc, x, arrow + y_ofst * 2);
		break;
	case MOVE_TYPE::Vertical_Throw_Up:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, "4.‰”’¼“Š‚°ã‚°‰^“®");
		TextOut(hdc, x, y2 + y_ofst * 3, str, strlen(str));
		RightArrow(hdc, x, arrow + y_ofst * 3);
		break;
	case MOVE_TYPE::Oblique_Throw:
		SetTextColor(hdc, ACTIVE);
		wsprintf(str, "5.Î•û“ŠË‰^“®");
		TextOut(hdc, x, y2 + y_ofst * 4, str, strlen(str));
		wsprintf(str, "“ŠËŠp");
		TextOut(hdc, x + 10, y2 + y_ofst * 5, str, strlen(str));
		snprintf(str, sizeof(str), ": %3.1f“x", Theta_);
		TextOut(hdc, x_ofst, y2 + y_ofst * 5, str, strlen(str));
		RightArrow(hdc, x, arrow + y_ofst * 4);
		LeftArrow(hdc, x + 60, arrow + y_ofst * 5);
		RightArrow(hdc, x + 90, arrow + y_ofst * 5);
		break;
	case MOVE_TYPE::END:
		break;
	default:
		break;
	}
}

void Viewer::RightArrow(HDC hdc, int x, int y)
{
	MoveToEx(hdc, x- 3, y, NULL);
	LineTo(hdc, x - 12, y - 6);
	LineTo(hdc, x - 12, y + 6);
	LineTo(hdc, x -  3, y);
}

void Viewer::LeftArrow(HDC hdc, int x, int y)
{
	MoveToEx(hdc, x + 3, y, NULL);
	LineTo(hdc, x + 12, y - 6);
	LineTo(hdc, x + 12, y + 6);
	LineTo(hdc, x + 3, y);
}
