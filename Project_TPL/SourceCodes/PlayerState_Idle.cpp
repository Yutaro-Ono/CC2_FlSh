#include "PlayerState_Idle.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

PlayerState_Idle::PlayerState_Idle()
{
	m_animSpeed = 0.5f;
}

PlayerState_Idle::~PlayerState_Idle()
{
}

PLAYER_STATE PlayerState_Idle::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// �ҋ@�X�e�[�g����A�������Ƃɑ��̃X�e�[�g�ֈڍs����
	//---------------------------------------------------------------------+

	// �o�C���h�����v���C���[�����Ԃ̎擾
	bool toggleWalk = _player->GetToggleWalk();
	bool toggleCrouch = _player->GetToggleCrouch();

	// �R���g���[���ڑ���
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �W�����v�����ւ̈ڍs
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_Y))
		{
			//return PLAYER_STATE::STATE_JUMP_START;
		}

		// �ҋ@��Ԃ�(�ړ��{�^����������Ă��邩)
		Vector2 move = CONTROLLER_INSTANCE.GetLAxisVec();
		float inputVal = move.LengthSq();

		// ���Ⴊ�ݏ��
		if (toggleCrouch)
		{
			return PLAYER_STATE::STATE_CROUCH;
		}

		// ���ȏ�̓��͒l�ŏ������Ԃֈڍs
		if (inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_JOG;
		}
		// �������
		if (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_WALK;
		}



	}

	// �R���g���[�����ڑ���
	else
	{
		// �W�����v�����ւ̈ڍs
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_SPACE))
		{
			return PLAYER_STATE::STATE_JUMP_START;
		}

		// �ҋ@��Ԃ�(�ړ��L�[��������Ă��邩)
		bool isIdle = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_W) &
			          INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_A) &
			          INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_S) &
			          INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_D);

		// ���Ⴊ�ݏ��
		if (toggleCrouch)
		{
			return PLAYER_STATE::STATE_CROUCH;
		}

		// �����g�O���L���������ꂩ�̓��̓L�[��������Ă����������Ԃ�
		if (toggleWalk && !isIdle)
		{
			return PLAYER_STATE::STATE_WALK;
		}
		// �ړ����Ă���ꍇ�A������
		if (!isIdle)
		{
			return PLAYER_STATE::STATE_JOG;
		}
	}



	return PLAYER_STATE::STATE_IDLE;
}

void PlayerState_Idle::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_IDLE), m_animSpeed);
}

