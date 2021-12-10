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
	// �ҋ@��Ԏ��A�����Ԏ��̂ݍ��G
	if (m_enemyOwner->GetEnemyState() == ENEMY_STATE::STATE_IDLE ||
		m_enemyOwner->GetEnemyState() == ENEMY_STATE::STATE_PATROL)
	{
		// ���o��Ԃ̍X�V
		DetectionActor();
	}

}

/// <summary>
/// ���o����^�[�Q�b�g�̃Z�b�g
/// </summary>
/// <param name="_target"> �^�[�Q�b�g����A�N�^�[�̃|�C���^ </param>
void DetectionActorComponent::SetTarget(Actor* _target)
{
	m_target = _target;
}

/// <summary>
/// �^�[�Q�b�g�v���C���[�̌��o����
/// </summary>
/// <returns> �^�[�Q�b�g���~�`�͈͓��ɑ��݂��邩 </returns>
void DetectionActorComponent::DetectionActor()
{
	// ���o�͈͂̍��W���X�V
	m_detectionArea.m_center = m_owner->GetPosition();

	// �^�[�Q�b�g�����݂��Ȃ�
	if (m_target == nullptr)
	{
		return;
	}

	// �^�[�Q�b�g���͈͓��ɑ��݂��邩�ǂ���
	bool prevIsDetected = m_isDetected;      // �X�V�O�̌��o�t���O
	m_isDetected = m_detectionArea.Contains(m_target->GetPosition());

	if (m_isDetected)
	{
		// �X�e�[�g��ύX
		m_enemyOwner->SetEnemyState(ENEMY_STATE::STATE_TRACK);
		return;
	}
}
