#pragma once
#include "Component.h"
#include "Math.h"


class TrackActorComponent : public Component
{

public:

	TrackActorComponent(class EnemyZombie* _owner);
	~TrackActorComponent();

	void Update(float _deltaTime) override;

	void TrackTarget(float _deltaTime);

private:

	class EnemyZombie* m_enemyOwner;

	Vector3 m_targetPos;          // �ǐՂ�����W
	Vector3 m_moveVec;            // �ړ�

	float m_moveSpeed;            // �ړ����x

	float m_trackMinRange;        // �ǐՂ̍ŏ�����
	float m_trackMaxRange;        // �ǐՂ̍ő勗��


};