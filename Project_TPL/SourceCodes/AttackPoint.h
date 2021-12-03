#pragma once
#include "Actor.h"

class AttackPoint : public Actor
{
public:

	AttackPoint(OBJECT_TAG _attackTag);
	~AttackPoint();



private:

	class BoxColliderComponent* m_attackBox;

};