#pragma once

#include <Windows.h>
#include "HermitianCurve.h"

class Laser
{
public:
	enum class STATUS {
		STANDBY,
		SHOOT,
		END
	};
public:
	Laser();
	~Laser();

	BOOL Create(const UINT curve_div,
		const POINTFLOAT s_pos, const POINTFLOAT s_vec,
		const POINTFLOAT e_pos, const POINTFLOAT e_vec);

	void Update();
	void Update(const POINTFLOAT s_pos, const POINTFLOAT e_pos);
	void Draw(HDC hdc);

public:
	//setter
	void SetStatus(STATUS st) { Status_ = st; }
	//getter
	const STATUS& GetStatus() const { return Status_; }

private:
	STATUS Status_;							// �X�e�[�^�X
	INT Idx_Start_;							// ���[�U�[�J�n�C���f�b�N�X
	INT Idx_End_;							// ���[�U�[�I���C���f�b�N�X
	HermitianCurve* HCurve_;
};
