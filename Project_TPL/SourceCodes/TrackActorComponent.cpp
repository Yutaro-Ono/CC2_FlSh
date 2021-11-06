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
	// �I�[�i�[�G�l�~�[���ǐՏ�Ԃ̎��̂ݍX�V
	if (m_enemyOwner->GetEnemyState() == ENEMY_STATE::STATE_PATROL)
	{
		// �v���C���[�̒ǔ�����
		TrackTarget(_deltaTime);

		// �G�l�~�[�̍ŏI���W���X�V
		Vector3 resultPos = m_enemyOwner->GetPosition() + m_moveVec;
		m_enemyOwner->SetPosition(resultPos);
	}
}

void TrackActorComponent::TrackTarget(float _deltaTime)
{
	// �^�[�Q�b�g���W�̍X�V
	m_targetPos = GAME_INSTANCE.GetPlayerActor()->GetPosition();

	// �G�l�~�[���v���C���[�̋����x�N�g��
	Vector3 enemyToActor = m_targetPos - m_enemyOwner->GetPosition();
	float length = enemyToActor.Length();

	// �ǐՍŏ������ɒB�����ꍇ�A�U�����[�h�ֈڍs
	if (length <= m_trackMinRange)
	{
		m_enemyOwner->SetEnemyState(ENEMY_STATE::STATE_ATTACK);
		return;
	}

	// �ǐՍő勗����藣�ꂽ�ꍇ�A���񃂁[�h�ֈڍs
	if (length >= m_trackMaxRange)
	{
		m_enemyOwner->SetEnemyState(ENEMY_STATE::STATE_PATROL);
		return;
	}

	// �G�l�~�[���v���C���[�̋����𐅕������ւ̌����ɕϊ�
	enemyToActor.z = 0.0f;
	enemyToActor.Normalize();

	// �ړ��x�N�g�����X�V
	m_moveVec = m_moveSpeed * enemyToActor * _deltaTime;
}
