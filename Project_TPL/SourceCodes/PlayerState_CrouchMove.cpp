#include "PlayerState_CrouchMove.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

PlayerState_CrouchMove::PlayerState_CrouchMove()
{
}

PlayerState_CrouchMove::~PlayerState_CrouchMove()
{
}

PLAYER_STATE PlayerState_CrouchMove::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// �����݃X�e�[�g����A�������Ƃɑ��̃X�e�[�g�ֈڍs����
	//---------------------------------------------------------------------+

	// �o�C���h�����v���C���[���瑖��/������Ԃ̎擾
	bool toggleSprint = _player->GetToggleSprint();
	bool toggleWalk = _player->GetToggleWalk();
	bool toggleCrouch = _player->GetToggleCrouch();

	// �R���g���[���ڑ���
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �W�����v�����ւ̈ڍs
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_A))
		{
			//return PLAYER_STATE::STATE_JUMP_START;
		}

		// �ҋ@��Ԃ�(�ړ��{�^����������Ă��邩)
		Vector2 move = CONTROLLER_INSTANCE.GetLAxisVec();
		float inputVal = move.LengthSq();

		// ���Ⴊ�݃g�O��������������ȏ�̃X�s�[�h
		if (!toggleCrouch && !toggleWalk && (inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE))
		{
			return PLAYER_STATE::STATE_JOG;
		}
		// ���Ⴊ�݃g�O�������������ȏ�̃X�s�[�h
		if (!toggleCrouch && (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE))
		{
			return PLAYER_STATE::STATE_WALK;
		}

		// ���͒l���������͒l�ȏ�ł����ݕ������
		if (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_CROUCH_MOVE;
		}
	}

	// �R���g���[�����ڑ���
	else
	{
		// �W�����v�����ւ̈ڍs
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_SPACE))
		{
			//return PLAYER_STATE::STATE_JUMP_START;
		}

		// �ҋ@��Ԃ�(�ړ��L�[��������Ă��邩)
		bool isIdle = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_W) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_A) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_S) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_D);

		// ���Ⴊ�݃g�O�������E�ҋ@��ԂłȂ��E�����g�O��ON
		if (!toggleCrouch && !isIdle && toggleWalk)
		{
			return PLAYER_STATE::STATE_WALK;
		}
		// ���Ⴊ�݃g�O�������E�ҋ@��ԂłȂ�
		if (!toggleCrouch && !isIdle)
		{
			return PLAYER_STATE::STATE_JOG;
		}

		// �����ꂩ�̓��̓L�[��������Ă����������Ԃ�
		if (!isIdle)
		{
			return PLAYER_STATE::STATE_CROUCH_MOVE;
		}

	}

	return PLAYER_STATE::STATE_CROUCH;
}

void PlayerState_CrouchMove::EnterState(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_CROUCH_MOVE), m_animSpeed * _deltaTime);
}
