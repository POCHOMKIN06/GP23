#pragma once

#include <Windows.h>

class HermitianCurve
{
public:
	typedef struct _Params
	{
		UINT c_div;
		POINTFLOAT *c_pt;
		POINTFLOAT s_pos;
		POINTFLOAT s_vec;
		POINTFLOAT e_pos;
		POINTFLOAT e_vec;
	} Params;
protected:
	HermitianCurve();
public:
	~HermitianCurve();

public:
	/*@brief : Use with SetParams(), Delete this at User side
	* @param[out] out : output pointer
	*/
	static void Create(HermitianCurve** out);
	/*@brief
	* @param[in] curve_div: divide num
	* @param[in] s_pos : start position
	* @param[in] s_vec : start vector
	* @param[in] e_pos : end position
	* @param[in] e_vec : end vector
	*/
	HRESULT SetParams(const UINT curve_div,
		const POINTFLOAT s_pos, const POINTFLOAT s_vec,
		const POINTFLOAT e_pos, const POINTFLOAT e_vec);
public://setter
	void SetDivNum(const UINT d_num)
	{
		delete[] Cal_Pt_;
		Curve_Div_ = d_num;
		Cal_Pt_ = new POINTFLOAT[Curve_Div_ + 1];
		Calculate();
	}
	void SetPoses(const POINTFLOAT s_pos, const POINTFLOAT e_pos)
	{
		Start_Pos_ = s_pos;
		End_Pos_ = e_pos;
		Calculate();
	}
	void SetSPos(const POINTFLOAT s_pos) { Start_Pos_ = s_pos; Calculate(); }
	void SetSVec(const POINTFLOAT s_vec) { Start_Vec_ = s_vec; Calculate(); }
	void SetEPos(const POINTFLOAT e_pos) { End_Pos_ = e_pos; Calculate(); }
	void SetEVec(const POINTFLOAT e_vec) { End_Vec_ = e_vec; Calculate(); }
public://getter
	const Params GetParams() const
	{
		Params ps{
			Curve_Div_,
			Cal_Pt_,
			Start_Pos_,
			Start_Vec_,
			End_Pos_,
			End_Vec_
		};
		return ps;
	}
private:
	void Calculate();
protected:
	UINT Curve_Div_;
	POINTFLOAT* Cal_Pt_;
	POINTFLOAT Start_Pos_;
	POINTFLOAT Start_Vec_;
	POINTFLOAT End_Pos_;
	POINTFLOAT End_Vec_;
};

