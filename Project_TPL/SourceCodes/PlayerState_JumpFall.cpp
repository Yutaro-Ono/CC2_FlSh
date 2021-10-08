#include "PlayerState_JumpFall.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"


PlayerState_JumpFall::PlayerState_JumpFall()
{
}

PlayerState_JumpFall::~PlayerState_JumpFall()
{
}

PLAYER_STATE PlayerState_JumpFall::Update(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();

	// ジャンプ開始アニメーションが終了したら次のステートへ
	if (!skel->IsPlaying())
	{
		return PLAYER_STATE::STATE_JUMP_LAND;
	}

	return PLAYER_STATE::STATE_JUMP_FALL;
}

void PlayerState_JumpFall::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_JUMP_FALL), m_animSpeed);
}
