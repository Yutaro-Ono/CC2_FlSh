#include "EnemyZombieState_Walk.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"

EnemyZombieState_Walk::EnemyZombieState_Walk()
{
	m_animSpeed = 1.25f;
}

EnemyZombieState_Walk::~EnemyZombieState_Walk()
{
}

ZOMBIE_STATE EnemyZombieState_Walk::Update(EnemyZombie* _zombie, float _deltaTime)
{

	// 追跡状態では走りステートを続行
	if (_zombie->GetEnemyState() == ENEMY_STATE::STATE_TRACK)
	{
		return ZOMBIE_STATE::STATE_WALK;
	}

	// 攻撃状態に入ったら攻撃ステートへ移行
	if (_zombie->GetEnemyState() == ENEMY_STATE::STATE_ATTACK)
	{
		return ZOMBIE_STATE::STATE_ATTACK;
	}

	// 巡回状態
	if (_zombie->GetEnemyState() == ENEMY_STATE::STATE_PATROL)
	{
		return ZOMBIE_STATE::STATE_IDLE;
	}

	return ZOMBIE_STATE::STATE_WALK;
}

void EnemyZombieState_Walk::EnterState(EnemyZombie* _zombie)
{
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	// 起き上がりモーションを再生
	// 再生を終了したら次のステートへ遷移(アニメーション再生中は起き上がりで固定される)
	skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_WALK), m_animSpeed);
}
