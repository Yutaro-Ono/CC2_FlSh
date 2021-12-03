#pragma once
#include "Component.h"
#include "Actor.h"

class AttackComponent : public Component
{
public:

	AttackComponent(Actor* _owner, OBJECT_TAG _attackTag);
	~AttackComponent();





private:

	class AttackPoint* m_attackPoint;

};