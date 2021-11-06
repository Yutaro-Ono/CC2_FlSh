#include "EnemyZombieState_Idle.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"
#include "DetectionActorComponent.h"

EnemyZombieState_Idle::EnemyZombieState_Idle()
	:m_motionNum(0)
{
}

EnemyZombieState_Idle::~EnemyZombieState_Idle()
{
}

ZOMBIE_STATE EnemyZombieState_Idle::Update(EnemyZombie* _zombie, float _deltaTime)
{
	// �]���r���^�[�Q�b�g�����o���A�T�[�`��Ԃɓ�����
	if (_zombie->GetEnemyState() == ENEMY_STATE::STATE_PATROL)
	{
		// �Q���ׂ��Ԃ��ǂ����ŕ���
		if (_zombie->GetIsLaying())
		{
			return ZOMBIE_STATE::STATE_LAY_WAKEUP;
		}

		//return ZOMBIE_STATE::STATE_RUN;
	}


	// �ҋ@���[�V�����̔h������
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	if (!skel->IsPlaying())
	{
		if (m_motionNum == 0)
		{
			// �ҋ@���[�V����1���Đ�
			skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_IDLE), m_animSpeed);
			m_motionNum = 1;
			return ZOMBIE_STATE::STATE_IDLE;
		}

		if (m_motionNum == 1)
		{
			// �ҋ@���[�V����2���Đ�
			skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_IDLE_2), m_animSpeed);
			m_motionNum = 0;
			return ZOMBIE_STATE::STATE_IDLE;
		}

	}

	return ZOMBIE_STATE::STATE_IDLE;
}

void EnemyZombieState_Idle::EnterState(EnemyZombie* _zombie)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// �Q�Ă��邩�ǂ����ŃA�j���[�V������ύX
	if (_zombie->GetIsLaying())
	{
		// �Q���ׂ胂�[�V�������Đ�
		// �N���オ�郂�[�V�����ɑJ�ڂ��Ȃ��悤�ɍĐ����x��0�ɐݒ�
		skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_LAY_WAKEUP), 0.0f);
	}
	else
	{
		// �ʏ�ҋ@���[�V�������Đ�
		skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_IDLE), m_animSpeed);
	}
}
