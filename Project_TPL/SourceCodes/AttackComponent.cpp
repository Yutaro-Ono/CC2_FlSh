#include "AttackComponent.h"


AttackComponent::AttackComponent(Actor* _owner, OBJECT_TAG _attackTag)
	:Component(_owner)
	,m_attackPoint(nullptr)
{
}

AttackComponent::~AttackComponent()
{
}
