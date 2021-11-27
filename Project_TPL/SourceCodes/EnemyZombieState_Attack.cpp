#include "EnemyZombieState_Attack.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"


EnemyZombieState_Attack::EnemyZombieState_Attack()
{
	m_animSpeed = 0.8f;
}

EnemyZombieState_Attack::~EnemyZombieState_Attack()
{
}

ZOMBIE_STATE EnemyZombieState_Attack::Update(EnemyZombie* _zombie, float _deltaTime)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// �U�����[�V�������I��������
	if (!skel->IsPlaying())
	{
		_zombie->SetEnemyState(ENEMY_STATE::STATE_TRACK);
		return ZOMBIE_STATE::STATE_WALK;
	}

	return ZOMBIE_STATE::STATE_ATTACK;
}

void EnemyZombieState_Attack::EnterState(EnemyZombie* _zombie)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// �N���オ�胂�[�V�������Đ�
	// �Đ����I�������玟�̃X�e�[�g�֑J��(�A�j���[�V�����Đ����͋N���オ��ŌŒ肳���)
	skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_ATTACK), m_animSpeed);
}
