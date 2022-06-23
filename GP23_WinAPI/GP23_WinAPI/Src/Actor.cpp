#include "actor.h"
//#include "game.h"
//#include "component.h"
#include <algorithm>

//------------------------------------------------------------------------------
Actor::Actor()
	: State_(State::ACTIVE)
	, Tag_(Tag::NON)
	, Pos_(POINT{0, 0})
	, Deg_(FLOAT{0.0f, 0.0f})
	, Scl_(POINTFLOAT{1.0f, 1.0f})
{
}

Actor::~Actor()
{
	//GetGame()->RemoveActor(this);
	// コンポーネントを削除する必要がある
	// Component_がRemoveComponentを呼び出すため、異なるスタイルのループが必要です。
	while (!Component_.empty()) {
		delete Component_.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (State_ == State::ACTIVE) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto&& comp : Component_) {
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput()
{
	if (State_ == State::ACTIVE) {
		for (auto&& comp : Component_) {
			comp->ComponentInput();
		}
		ActorInput();
	}
}

void Actor::ActorInput()
{
}

bool Actor::IsValid()
{
	if (State_ != State::DEAD || State_ != State::MAX_STATE) {
		return true;
	}
	return false;
}

void Actor::AddCompornent(Component * component)
{
	// ソート済みの配列で挿入点を見つける
	// (それは自分よりも順位の高い最初の要素の位置のこと)
	int myOrder = component->GetUpdateOrder();
	auto iter = Component_.begin();
	for (;
		iter != Component_.end();
		++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	// イテレータの位置より前に要素を挿入する
	Component_.insert(iter, component);
}

void Actor::RemoveComponent(Component * component)
{
	auto iter = std::find(Component_.begin(), Component_.end(), component);
	if (iter != Component_.end()) {
		Component_.erase(iter);
	}
}

DirectX::XMFLOAT2 Actor::Get2DForward() const
{
	return DirectX::XMFLOAT2(
		cosf(Deg_.z*(Pi / 180)),
		sinf(Deg_.z*(Pi / 180)));
}
