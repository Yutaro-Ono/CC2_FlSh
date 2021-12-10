#include "DetectionActorComponent.h"
#include "EnemyBase.h"


DetectionActorComponent::DetectionActorComponent(class EnemyBase* _owner, class Actor* _target)
	:Component(_owner)
	,m_enemyOwner(_owner)
	,m_target(_target)
	,m_isDetected(false)
	,m_detectionRadius(20.0f)
	,m_detectionArea(m_owner->GetPosition(), 180.0f)
{
}

DetectionActorComponent::~DetectionActorComponent()
{
}

void DetectionActorComponent::Update(float _deltaTime)
{
	// 待機状態時、巡回状態時のみ索敵
	if (m_enemyOwner->GetEnemyState() == ENEMY_STATE::STATE_IDLE ||
		m_enemyOwner->GetEnemyState() == ENEMY_STATE::STATE_PATROL)
	{
		// 検出状態の更新
		DetectionActor();
	}

}

/// <summary>
/// 検出するターゲットのセット
/// </summary>
/// <param name="_target"> ターゲットするアクターのポインタ </param>
void DetectionActorComponent::SetTarget(Actor* _target)
{
	m_target = _target;
}

/// <summary>
/// ターゲットプレイヤーの検出処理
/// </summary>
/// <returns> ターゲットが円形範囲内に存在するか </returns>
void DetectionActorComponent::DetectionActor()
{
	// 検出範囲の座標を更新
	m_detectionArea.m_center = m_owner->GetPosition();

	// ターゲットが存在しない
	if (m_target == nullptr)
	{
		return;
	}

	// ターゲットが範囲内に存在するかどうか
	bool prevIsDetected = m_isDetected;      // 更新前の検出フラグ
	m_isDetected = m_detectionArea.Contains(m_target->GetPosition());

	if (m_isDetected)
	{
		// ステートを変更
		m_enemyOwner->SetEnemyState(ENEMY_STATE::STATE_TRACK);
		return;
	}
}
