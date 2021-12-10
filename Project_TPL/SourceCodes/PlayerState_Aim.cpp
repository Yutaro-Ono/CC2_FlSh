#include "PlayerState_Aim.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"
#include "WeaponAR4.h"

PlayerState_Aim::PlayerState_Aim()
{
	m_animSpeed = 1.5f;
}

PlayerState_Aim::~PlayerState_Aim()
{
}

PLAYER_STATE PlayerState_Aim::Update(Player* _player, float _deltaTime)
{

	// 右クリックを離した時、エイム状態を解除
	if (!MOUSE_INSTANCE.GetRightButtonValue(Mouse::MouseButtonState::MOUSE_BUTTON_PRESSED))
	{
		// カメラを変更
		_player->SetActiveAimCamera(false);

		return PLAYER_STATE::STATE_WEAPONOUT_IDLE;
	}

	return PLAYER_STATE::STATE_AIM;
}

void PlayerState_Aim::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	// しゃがみ状態かどうかでモーションを変更
	if (_player->GetToggleCrouch())
	{
		// しゃがみエイムアニメの再生
		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_AIM_CROUCH), m_animSpeed);
	}
	else
	{
		// 立ちエイムアニメの再生
		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_AIM_STAND), m_animSpeed);
	}

	// エイム用の一人称視点カメラに変更
	_player->SetActiveAimCamera(true);
}
