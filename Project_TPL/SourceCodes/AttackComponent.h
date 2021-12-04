#pragma once
#include "Component.h"
#include "Actor.h"

class AttackComponent : public Component
{
public:

	enum class ATTACK_STATE
	{
		NONE = 0,
		START_ATTACK,
		IN_ATTACK,
		AFTER_STOP
	};

	AttackComponent(Actor* _owner, OBJECT_TAG _attackTag, int _updateOrder = 100);
	~AttackComponent();

	void Update(float _deltaTime) override;

	void SetAllAttackFrame(float _occure, float _duration, float _stop);
	
	void AttackStart();


private:

	ATTACK_STATE m_attackState;
	class AttackPoint* m_attackPoint;

	float m_frameCounter;             // フレーム計算用

	float m_frameOccurrence;          // 入力～攻撃判定が発生するまでのフレーム
	float m_frameAttackDuration;      // 攻撃判定が持続するフレーム
	float m_frameStop;                // 攻撃後停止フレーム

};