#include "EnemyBase.h"

EnemyBase::EnemyBase(OBJECT_TAG _tag)
	:Actor(_tag)
	,m_health(10)
	,m_enemyState(ENEMY_STATE::STATE_IDLE)
{
}

EnemyBase::~EnemyBase()
{
}


/// <summary>
/// 体力を増加する処理
/// </summary>
/// <param name="_addVal"></param>
void EnemyBase::AddHealth(unsigned short int _addVal)
{
	unsigned short int healthVal = m_health + _addVal;

	// 変数のオーバーフロー防止
	if (healthVal > SHRT_MAX)
	{
		m_health = SHRT_MAX;
		return;
	}

	m_health += _addVal;

}

/// <summary>
/// 体力を減らす処理
/// </summary>
/// <param name="_redVal"></param>
void EnemyBase::ReduceHealth(unsigned short int _redVal)
{
	int healthVal = m_health - _redVal;

	// 変数のオーバーフロー対策
	if (healthVal < SHRT_MIN)
	{
		m_health = SHRT_MIN;
		return;
	}

	m_health -= _redVal;

}
