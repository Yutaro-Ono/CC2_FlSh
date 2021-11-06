#include "EnemyZombieState_WakeUp.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

EnemyZombieState_WakeUp::EnemyZombieState_WakeUp()
{
}

EnemyZombieState_WakeUp::~EnemyZombieState_WakeUp()
{
}

ZOMBIE_STATE EnemyZombieState_WakeUp::Update(EnemyZombie* _zombie, float _deltaTime)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// アニメーション再生終了したら次のステートへ
	if (!skel->IsPlaying())
	{
		return ZOMBIE_STATE::STATE_IDLE;
	}

    return ZOMBIE_STATE::STATE_LAY_WAKEUP;
}

void EnemyZombieState_WakeUp::EnterState(EnemyZombie* _zombie)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// 起き上がりモーションを再生
    // 再生を終了したら次のステートへ遷移(アニメーション再生中は起き上がりで固定される)
	skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_LAY_WAKEUP), m_animSpeed);
}
