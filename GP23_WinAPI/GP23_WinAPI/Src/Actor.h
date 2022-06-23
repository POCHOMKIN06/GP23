#pragma once

#include <vector>
#include <cstdint>


#include <Windows.h>

const float Pi = 3.1415926535f;
const float TwoPi = Pi * 2.0f;

//-----------------------------------------------------------------------------
//	Actorの基底クラス
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
//基本処理
public:
	Actor();
	virtual ~Actor();

			void Update(float deltaTime);			// Gameから呼び出される更新関数(不可:オーバーライド)
	//for Update**
			void UpdateComponents(float deltaTime);	// Actorが持つ全Componentを更新(不可:オーバーライド)
	virtual void UpdateActor(float deltaTime);		// Actor独自の更新処理(可:オーバライド)

	//引数にconst WORD* keystateを入れて処理する方法もあるが、
	//今回はinput.hで処理を代替しているので、書き換える場合は詳細を見ること
			void ProcessInput();// ProcessInput関数は、Gameから呼び出される(不可:オーバーライド)
	//for ProcessInput*
	virtual void ActorInput();	// Actor独自のInput用コード(可:オーバーライド)
	bool IsValid();
//
//所属情報
//Component
public:
	//ComponentをActorへ紐づけ(Componentで呼び出し)
	void AddCompornent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	// Actorの所有するComponent
	std::vector<class Component*> Component_;
	
//
//Actor Parameter
	//Getter
	State				GetState()	const { return State_; };
	Tag					GetTag()	const { return Tag_; };
	const POINT			GetPos()	const { return Pos_; };
	const FLOAT			GetRot()	const { return Deg_; };
	const POINTFLOAT	GetScl()	const { return Scl_; };
	//Actorの向いている方向取得
	//DirectX::XMFLOAT2 Get2DForward() const;	//degree to radian
	//Setter
	void SetState(State state) { State_ = state; };
	void SetTag(Tag tag) { Tag_ = tag; };
	void SetPos(const POINT pos) { Pos_ = pos; };
	void SetRot(const FLOAT& rot) { Deg_ = rot; };
	void SetScl(const POINTFLOAT& scl) { Scl_ = scl; };
protected:
	// アクターの状態
	State State_;
	Tag	Tag_;
	// 座標変換(Transform)
	POINT Pos_;
	FLOAT Deg_;
	POINTFLOAT Scl_;
};

