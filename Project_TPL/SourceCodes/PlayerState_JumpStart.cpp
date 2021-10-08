#include "PlayerState_JumpStart.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

PlayerState_JumpStart::PlayerState_JumpStart()
{
	m_animSpeed = 15.0f;
}

PlayerState_JumpStart::~PlayerState_JumpStart()
{
}

PLAYER_STATE PlayerState_JumpStart::Update(Player* _player, float _deltaTime)
{

	// ジャンプ開始アニメーションが終了したら次のステートへ
	if (!_player->GetSkelMesh()->IsPlaying())
	{
		return PLAYER_STATE::STATE_JUMP_FALL;
	}
	
	return PLAYER_STATE::STATE_JUMP_START;
}

void PlayerState_JumpStart::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_JUMP_START), m_animSpeed);
}
