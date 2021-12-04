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

	float m_frameCounter;             // �t���[���v�Z�p

	float m_frameOccurrence;          // ���́`�U�����肪��������܂ł̃t���[��
	float m_frameAttackDuration;      // �U�����肪��������t���[��
	float m_frameStop;                // �U�����~�t���[��

};