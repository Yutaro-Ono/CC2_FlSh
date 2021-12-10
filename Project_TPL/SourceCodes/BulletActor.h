#pragma once
#include "Actor.h"

class BulletActor : public Actor
{
public:

	BulletActor(const Vector3& _shootDirVec);
	~BulletActor();

	void UpdateActor(float _deltaTime) override;

	void OnCollisionEnter(class ColliderComponent* _ownCollComp, class ColliderComponent* _otherCollComp) override;

private:

	float m_velocity;
	Vector3 m_dirVec;

};