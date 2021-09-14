#include "PlayerState_Crouch.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

PlayerState_Crouch::PlayerState_Crouch()
{
}

PlayerState_Crouch::~PlayerState_Crouch()
{
}

PLAYER_STATE PlayerState_Crouch::Update(Player* _player, float _deltaTime)
{

	//---------------------------------------------------------------------+
    // �����݃X�e�[�g����A�������Ƃɑ��̃X�e�[�g�ֈڍs����
    //---------------------------------------------------------------------+

	// �o�C���h�����v���C���[���瑖��/������Ԃ̎擾
	bool toggleSprint = _player->GetToggleSprint();
	bool toggleWalk = _player->GetToggleWalk();

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

		// ������
		if (toggleSprint)
		{
			return PLAYER_STATE::STATE_SPRINT;
		}
		// ���͒l���������͒l�ȏ�ł����ݕ������
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
			//return PLAYER_STATE::STATE_JUMP_START;
		}

		// �ҋ@��Ԃ�(�ړ��L�[��������Ă��邩)
		bool isIdle = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_W) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_A) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_S) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_D);

		// �����ꂩ�̈ړ��L�[������+���V�t�g��������Ă�����A�����Ԃֈڍs
		if (toggleSprint && !isIdle)
		{
			return PLAYER_STATE::STATE_SPRINT;
		}
		// �����ꂩ�̓��̓L�[��������Ă����������Ԃ�
		if (!isIdle)
		{
			return PLAYER_STATE::STATE_CROUCH_MOVE;
		}

	}





	return PLAYER_STATE::STATE_CROUCH;
}

void PlayerState_Crouch::EnterState(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_CROUCH), m_animSpeed * _deltaTime);
}
