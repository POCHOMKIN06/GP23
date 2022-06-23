#pragma once

#include "Laser.h"

class Fireworks : public Laser
{
public:
	Fireworks();
	~Fireworks();

	void Update();
	void Update(const POINTFLOAT s_pos, const POINTFLOAT e_pos);

	void Draw(HDC hdc);

private:
	static const POINTFLOAT defSPos;
	static const POINTFLOAT defEPos;
	static const POINTFLOAT defSVec;
	static const POINTFLOAT defEVec;

	//Particle* Explosion_;
	//Particle* Spark_;
};

//s_pos��e_pos�������_���Ō���
//���̑����𑾂�������ׂ������肷��	**
//STATUS::END�Ńp�[�e�B�N������
//�����A�ΉԂ�2��ނ����
//�ϋq�Ȃ����
//�w�i���ɂ���
//�������
