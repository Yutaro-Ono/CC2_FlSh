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
	// ゾンビがターゲットを検出し、サーチ状態に入った
	if (_zombie->GetEnemyState() == ENEMY_STATE::STATE_PATROL)
	{
		// 寝そべり状態かどうかで分岐
		if (_zombie->GetIsLaying())
		{
			return ZOMBIE_STATE::STATE_LAY_WAKEUP;
		}

		//return ZOMBIE_STATE::STATE_RUN;
	}


	// 待機モーションの派生処理
	SkeletalMeshComponent* skel = _zombie->GetSkelMesh();

	if (!skel->IsPlaying())
	{
		if (m_motionNum == 0)
		{
			// 待機モーション1を再生
			skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_IDLE), m_animSpeed);
			m_motionNum = 1;
			return ZOMBIE_STATE::STATE_IDLE;
		}

		if (m_motionNum == 1)
		{
			// 待機モーション2を再生
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

	// 寝ているかどうかでアニメーションを変更
	if (_zombie->GetIsLaying())
	{
		// 寝そべりモーションを再生
		// 起き上がるモーションに遷移しないように再生速度を0に設定
		skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_LAY_WAKEUP), 0.0f);
	}
	else
	{
		// 通常待機モーションを再生
		skel->PlayAnimation(_zombie->GetAnim(ZOMBIE_STATE::STATE_IDLE), m_animSpeed);
	}
}
