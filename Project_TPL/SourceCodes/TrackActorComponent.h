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

	Vector3 m_targetPos;          // 追跡する座標
	Vector3 m_moveVec;            // 移動

	float m_moveSpeed;            // 移動速度

	float m_trackMinRange;        // 追跡の最小距離
	float m_trackMaxRange;        // 追跡の最大距離


};