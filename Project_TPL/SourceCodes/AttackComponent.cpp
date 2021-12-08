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
	// 攻撃当たり判定用アクターの生成
	m_attackPoint = new AttackPoint(_attackTag, Vector3(20.0f, -40.0f, -10.0f), Vector3(60.0f, 50.0f, 10.0f));


}

AttackComponent::~AttackComponent()
{
}

void AttackComponent::Update(float _deltaTime)
{
	// オーナーアクターに水平座標と回転行列を合わせる
	m_attackPoint->SetPosition(m_owner->GetPosition() + Vector3(0.0f, 0.0f, 30.0f));
	m_attackPoint->SetRotation(m_owner->GetRotation());


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

			// 当たり判定有効化
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

			// 当たり判定無効化
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
/// 攻撃処理の開始
/// </summary>
void AttackComponent::AttackStart()
{
	if (m_attackState != ATTACK_STATE::NONE)
	{
		return;
	}

	// 攻撃ステートを開始状態に
	m_attackState = ATTACK_STATE::START_ATTACK;
	m_frameCounter = m_frameOccurrence;
}

