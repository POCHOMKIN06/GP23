#pragma once

#include <vector>
#include <cstdint>


#include <Windows.h>

const float Pi = 3.1415926535f;
const float TwoPi = Pi * 2.0f;

//-----------------------------------------------------------------------------
//	Actor�̊��N���X
//-----------------------------------------------------------------------------
class Actor
{
public:
	enum class State {
		ACTIVE,
		PAUSE,
		DEAD,

		MAX_STATE
	};
	enum class Tag {
		NON,
		TESTER,
		PLAYER,
		ENEMY,
		STRUCTURE,
		PROP,
		ITEM,
		PROJECTILE,
		P_PROJECTILE,
		E_PROJECTILE,
		EFFECT,
		UI,
		TEXT,
		COLLISION,

		MAX_TAG
	};


//
//��{����
public:
	Actor();
	virtual ~Actor();

			void Update(float deltaTime);			// Game����Ăяo�����X�V�֐�(�s��:�I�[�o�[���C�h)
	//for Update**
			void UpdateComponents(float deltaTime);	// Actor�����SComponent���X�V(�s��:�I�[�o�[���C�h)
	virtual void UpdateActor(float deltaTime);		// Actor�Ǝ��̍X�V����(��:�I�[�o���C�h)

	//������const WORD* keystate�����ď���������@�����邪�A
	//�����input.h�ŏ������ւ��Ă���̂ŁA����������ꍇ�͏ڍׂ����邱��
			void ProcessInput();// ProcessInput�֐��́AGame����Ăяo�����(�s��:�I�[�o�[���C�h)
	//for ProcessInput*
	virtual void ActorInput();	// Actor�Ǝ���Input�p�R�[�h(��:�I�[�o�[���C�h)
	bool IsValid();
//
//�������
//Component
public:
	//Component��Actor�֕R�Â�(Component�ŌĂяo��)
	void AddCompornent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	// Actor�̏��L����Component
	std::vector<class Component*> Component_;
	
//
//Actor Parameter
	//Getter
	State				GetState()	const { return State_; };
	Tag					GetTag()	const { return Tag_; };
	const POINT			GetPos()	const { return Pos_; };
	const FLOAT			GetRot()	const { return Deg_; };
	const POINTFLOAT	GetScl()	const { return Scl_; };
	//Actor�̌����Ă�������擾
	//DirectX::XMFLOAT2 Get2DForward() const;	//degree to radian
	//Setter
	void SetState(State state) { State_ = state; };
	void SetTag(Tag tag) { Tag_ = tag; };
	void SetPos(const POINT pos) { Pos_ = pos; };
	void SetRot(const FLOAT& rot) { Deg_ = rot; };
	void SetScl(const POINTFLOAT& scl) { Scl_ = scl; };
protected:
	// �A�N�^�[�̏��
	State State_;
	Tag	Tag_;
	// ���W�ϊ�(Transform)
	POINT Pos_;
	FLOAT Deg_;
	POINTFLOAT Scl_;
};

