#pragma once
#include "Actor.h"

class AttackPoint : public Actor
{
public:

	AttackPoint(OBJECT_TAG _attackTag, const Vector3& _sizeMin, const Vector3& _sizeMax);
	~AttackPoint();

	void UpdateActor(float _deltaTime) override;

	void OnCollisionEnter(ColliderComponent* _ownCollComp, ColliderComponent* _otherCollComp) override;

	void SetSizeOfAttackBox(const Vector3& _sizeMin, const Vector3& _sizeMax);

	void EnableCollision();
	void DisableCollision();

private:

	class BoxColliderComponent* m_attackBox;


};