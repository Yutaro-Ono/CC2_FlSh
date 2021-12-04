#include "AttackComponent.h"
#include "AttackPoint.h"

AttackComponent::AttackComponent(Actor* _owner, OBJECT_TAG _attackTag, int _updateOrder)
	:Component(_owner, _updateOrder)
	,m_attackState(ATTACK_STATE::NONE)
	,m_attackPoint(nullptr)
	,m_frameCounter(0.0f)
	,m_frameOccurrence(1.0f)
	,m_frameAttackDuration(1.0f)
	,m_frameStop(1.0f)
{
	m_attackPoint = new AttackPoint(_attackTag, Vector3(-30.0f, -300.0f, -300.0f), Vector3(30.0f, 300.0f, 300.0f));


}

AttackComponent::~AttackComponent()
{
}

void AttackComponent::Update(float _deltaTime)
{
	// �I�[�i�[�A�N�^�[�ƍ��W�����킹��
	m_attackPoint->SetPosition(m_owner->GetPosition());

	if (m_attackState == ATTACK_STATE::NONE)
	{
		return;
	}

	if (m_attackState == ATTACK_STATE::START_ATTACK)
	{
		m_frameCounter -= 1.0f * _deltaTime;

		if (m_frameCounter <= 0.0f)
		{
			m_frameCounter = m_frameAttackDuration;
			m_attackState = ATTACK_STATE::IN_ATTACK;

			// �����蔻��L����
			m_attackPoint->EnableCollision();

			return;
		}
	}

	if (m_attackState == ATTACK_STATE::IN_ATTACK)
	{
		m_frameCounter -= 1.0f * _deltaTime;

		if (m_frameCounter <= 0.0f)
		{
			m_frameCounter = m_frameStop;
			m_attackState = ATTACK_STATE::AFTER_STOP;

			// �����蔻�薳����
			m_attackPoint->DisableCollision();

			return;
		}
	}

	if (m_attackState == ATTACK_STATE::AFTER_STOP)
	{
		m_frameCounter -= 1.0f * _deltaTime;

		if (m_frameCounter <= 0.0f)
		{
			m_frameCounter = m_frameAttackDuration;
			m_attackState = ATTACK_STATE::NONE;

			return;
		}
	}
}

void AttackComponent::SetAllAttackFrame(float _occure, float _duration, float _stop)
{
	m_frameOccurrence = _occure;
	m_frameAttackDuration = _duration;
	m_frameStop = _stop;
}

/// <summary>
/// �U�������̊J�n
/// </summary>
void AttackComponent::AttackStart()
{
	if (m_attackState != ATTACK_STATE::NONE)
	{
		return;
	}

	// �U���X�e�[�g���J�n��Ԃ�
	m_attackState = ATTACK_STATE::START_ATTACK;
	m_frameCounter = m_frameOccurrence;
}
