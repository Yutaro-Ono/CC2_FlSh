#include "EnemyZombieState_Idle.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

EnemyZombieState_Idle::EnemyZombieState_Idle()
{
}

EnemyZombieState_Idle::~EnemyZombieState_Idle()
{
}

ZOMBIE_STATE EnemyZombieState_Idle::Update(EnemyZombie* _player, float _deltaTime)
{
	return ZOMBIE_STATE::STATE_IDLE;
}

void EnemyZombieState_Idle::EnterState(EnemyZombie* _zombie)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();
	skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_IDLE), m_animSpeed);
}
