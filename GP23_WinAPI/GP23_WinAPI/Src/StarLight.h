#pragma once

#include <Windows.h>

class StarLight
{
public:
	enum class SIZE {
		Small,
		Middle,
		Large,
		SIZE_Num
	};
public:
	StarLight(POINT pos, SIZE size);
	~StarLight();

	void Update(int time);
	void Draw(HDC hdc);

private:
	void Init(SIZE size);
private:
	SIZE size_;
	POINT pos_;
	int radius_;
	POINT poly_pos_[8];
};
