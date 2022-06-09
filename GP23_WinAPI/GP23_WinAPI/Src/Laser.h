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
	STATUS Status_;							// ステータス
	INT Idx_Start_;							// レーザー開始インデックス
	INT Idx_End_;							// レーザー終了インデックス
	HermitianCurve* HCurve_;
};
