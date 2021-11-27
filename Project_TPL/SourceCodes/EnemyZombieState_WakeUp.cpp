#include "EnemyZombieState_WakeUp.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

EnemyZombieState_WakeUp::EnemyZombieState_WakeUp()
{
	m_animSpeed = 1.0f;
}

EnemyZombieState_WakeUp::~EnemyZombieState_WakeUp()
{
}

ZOMBIE_STATE EnemyZombieState_WakeUp::Update(EnemyZombie* _zombie, float _deltaTime)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// �A�j���[�V�����Đ��I�������玟�̃X�e�[�g��
	if (!skel->IsPlaying())
	{
		// �N���オ��t���O��ON
		_zombie->SetIsLaying(false);
		// �ʏ�X�e�[�g��
		return ZOMBIE_STATE::STATE_WALK;
	}

    return ZOMBIE_STATE::STATE_LAY_WAKEUP;
}

void EnemyZombieState_WakeUp::EnterState(EnemyZombie* _zombie)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// �N���オ�胂�[�V�������Đ�
    // �Đ����I�������玟�̃X�e�[�g�֑J��(�A�j���[�V�����Đ����͋N���オ��ŌŒ肳���)
	skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_LAY_WAKEUP), m_animSpeed);
}
