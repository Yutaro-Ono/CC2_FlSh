#include "TrackActorComponent.h"
#include "EnemyBase.h"
#include "GameMain.h"

TrackActorComponent::TrackActorComponent(EnemyBase* _owner)
	:Component(_owner)
	,m_enemyOwner(_owner)
	,m_targetPos(Vector3::Zero)
	,m_moveSpeed(110.0f)
	,m_moveVec(Vector3::Zero)
	,m_trackMinRange(40.0f)
	,m_trackMaxRange(600.0f)
{
}

TrackActorComponent::~TrackActorComponent()
{
}

void TrackActorComponent::Update(float _deltaTime)
{
	// オーナーエネミーが追跡状態の時のみ更新
	if (m_enemyOwner->GetEnemyState() == ENEMY_STATE::STATE_PATROL)
	{
		// プレイヤーの追尾処理
		TrackTarget(_deltaTime);

		// エネミーの最終座標を更新
		Vector3 resultPos = m_enemyOwner->GetPosition() + m_moveVec;
		m_enemyOwner->SetPosition(resultPos);
	}
}

void TrackActorComponent::TrackTarget(float _deltaTime)
{
	// ターゲット座標の更新
	m_targetPos = GAME_INSTANCE.GetPlayerActor()->GetPosition();

	// エネミー→プレイヤーの距離ベクトル
	Vector3 enemyToActor = m_targetPos - m_enemyOwner->GetPosition();
	float length = enemyToActor.Length();

	// 追跡最小距離に達した場合、攻撃モードへ移行
	if (length <= m_trackMinRange)
	{
		m_enemyOwner->SetEnemyState(ENEMY_STATE::STATE_ATTACK);
		return;
	}

	// 追跡最大距離より離れた場合、巡回モードへ移行
	if (length >= m_trackMaxRange)
	{
		m_enemyOwner->SetEnemyState(ENEMY_STATE::STATE_PATROL);
		return;
	}

	// エネミー→プレイヤーの距離を水平方向への向きに変換
	enemyToActor.z = 0.0f;
	enemyToActor.Normalize();

	// 移動ベクトルを更新
	m_moveVec = m_moveSpeed * enemyToActor * _deltaTime;
}
