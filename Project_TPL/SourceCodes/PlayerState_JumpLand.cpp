#include "PlayerState_JumpLand.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"


PlayerState_JumpLand::PlayerState_JumpLand()
{
}

PlayerState_JumpLand::~PlayerState_JumpLand()
{
}

PLAYER_STATE PlayerState_JumpLand::Update(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();

	// �W�����v�J�n�A�j���[�V�������I�������玟�̃X�e�[�g��
	if (!skel->IsPlaying())
	{
		return PLAYER_STATE::STATE_IDLE;
	}

	return PLAYER_STATE::STATE_JUMP_LAND;
}

void PlayerState_JumpLand::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_JUMP_LAND), m_animSpeed);
}
