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
/// �̗͂𑝉����鏈��
/// </summary>
/// <param name="_addVal"></param>
void EnemyBase::AddHealth(unsigned short int _addVal)
{
	unsigned short int healthVal = m_health + _addVal;

	// �ϐ��̃I�[�o�[�t���[�h�~
	if (healthVal > SHRT_MAX)
	{
		m_health = SHRT_MAX;
		return;
	}

	m_health += _addVal;

}

/// <summary>
/// �̗͂����炷����
/// </summary>
/// <param name="_redVal"></param>
void EnemyBase::ReduceHealth(unsigned short int _redVal)
{
	int healthVal = m_health - _redVal;

	// �ϐ��̃I�[�o�[�t���[�΍�
	if (healthVal < SHRT_MIN)
	{
		m_health = SHRT_MIN;
		return;
	}

	m_health -= _redVal;

}
