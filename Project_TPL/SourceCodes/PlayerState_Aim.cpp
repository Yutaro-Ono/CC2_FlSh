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

	// �E�N���b�N�𗣂������A�G�C����Ԃ�����
	if (!MOUSE_INSTANCE.GetRightButtonValue(Mouse::MouseButtonState::MOUSE_BUTTON_PRESSED))
	{
		// �J������ύX
		_player->SetActiveAimCamera(false);

		return PLAYER_STATE::STATE_WEAPONOUT_IDLE;
	}

	return PLAYER_STATE::STATE_AIM;
}

void PlayerState_Aim::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	// ���Ⴊ�ݏ�Ԃ��ǂ����Ń��[�V������ύX
	if (_player->GetToggleCrouch())
	{
		// ���Ⴊ�݃G�C���A�j���̍Đ�
		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_AIM_CROUCH), m_animSpeed);
	}
	else
	{
		// �����G�C���A�j���̍Đ�
		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_AIM_STAND), m_animSpeed);
	}

	// �G�C���p�̈�l�̎��_�J�����ɕύX
	_player->SetActiveAimCamera(true);
}
