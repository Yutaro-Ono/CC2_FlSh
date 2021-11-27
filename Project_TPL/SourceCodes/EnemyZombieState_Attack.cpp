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

	// 攻撃モーションが終了したら
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

	// 起き上がりモーションを再生
	// 再生を終了したら次のステートへ遷移(アニメーション再生中は起き上がりで固定される)
	skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_ATTACK), m_animSpeed);
}
