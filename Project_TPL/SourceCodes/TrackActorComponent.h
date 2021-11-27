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

	Vector3 m_targetPos;          // ’ÇÕ‚·‚éÀ•W
	Vector3 m_moveVec;            // ˆÚ“®

	float m_moveSpeed;            // ˆÚ“®‘¬“x

	float m_trackMinRange;        // ’ÇÕ‚ÌÅ¬‹——£
	float m_trackMaxRange;        // ’ÇÕ‚ÌÅ‘å‹——£


};