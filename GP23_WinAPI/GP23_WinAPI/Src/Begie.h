#pragma once

#include <Windows.h>

class Begie
{
public:
	Begie()
		: Devide_(100)
	{
		Pos_[0] = { 100.0f, 400.0f };	//�n�_
		Pos_[1] = { 200.0f, 100.0f };	//���ԓ_
		Pos_[2] = { 500.0f, 200.0f };	//���ԓ_
		Pos_[3] = { 600.0f, 550.0f };	//�I�_
		DevPoint_ = new POINTFLOAT[Devide_+1];
		ZeroMemory(DevPoint_, Devide_);
	}
	~Begie() {};

	void Draw(HDC hdc);
private:
	UINT Devide_;
	POINTFLOAT Pos_[4];
	POINTFLOAT* DevPoint_;
};

