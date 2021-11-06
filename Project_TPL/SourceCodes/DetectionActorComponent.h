//----------------------------------------------------------------------------------+
// @file        DetectionActorComponent.h
// @brief       アクターを検出するコンポーネント
// @note        円形の索敵範囲に侵入したかを検出
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 11/05   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Component.h"


class DetectionActorComponent : public Component
{

public:

	DetectionActorComponent(class EnemyBase* _owner, class Actor* _target);
	~DetectionActorComponent();

	void Update(float _deltaTime) override;

	void SetTarget(class Actor* _target);

	bool GetIsDetected() { return m_isDetected; }

private:

	void DetectionActor();

	class EnemyBase* m_enemyOwner;    // オーナーエネミー
	class Actor* m_target;            // 検出対象アクター

	class Sphere* m_detectionArea;    // 検出範囲(円形)
	float m_detectionRadius;          // 検出範囲の半径


	bool m_isDetected;                // 検出できたかどうか
};