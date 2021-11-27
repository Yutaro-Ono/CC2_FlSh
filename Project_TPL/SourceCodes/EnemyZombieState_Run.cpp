#include "EnemyZombieState_Run.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

EnemyZombieState_Run::EnemyZombieState_Run()
{
}

EnemyZombieState_Run::~EnemyZombieState_Run()
{
}

ZOMBIE_STATE EnemyZombieState_Run::Update(EnemyZombie* _zombie, float _deltaTime)
{

	// �ǐՏ�Ԃł͑���X�e�[�g�𑱍s
	if (_zombie->GetEnemyState() == ENEMY_STATE::STATE_TRACK)
	{
		return ZOMBIE_STATE::STATE_RUN;
	}

	// �U����Ԃɓ�������U���X�e�[�g�ֈڍs
	if (_zombie->GetEnemyState() == ENEMY_STATE::STATE_ATTACK)
	{
		//return ZOMBIE_STATE::STATE_ATTACK;
	}

	// ������
	if (_zombie->GetEnemyState() == ENEMY_STATE::STATE_PATROL)
	{
		//return ZOMBIE_STATE::STATE_IDLE;
	}

	return ZOMBIE_STATE::STATE_RUN;
	
}

void EnemyZombieState_Run::EnterState(EnemyZombie* _zombie)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// �N���オ�胂�[�V�������Đ�
	// �Đ����I�������玟�̃X�e�[�g�֑J��(�A�j���[�V�����Đ����͋N���オ��ŌŒ肳���)
	skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_RUN), m_animSpeed);
}
