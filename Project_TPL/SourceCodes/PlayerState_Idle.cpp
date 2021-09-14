#include "PlayerState_Idle.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

PlayerState_Idle::PlayerState_Idle()
{
	m_animSpeed = 10.0f;
}

PlayerState_Idle::~PlayerState_Idle()
{
}

PLAYER_STATE PlayerState_Idle::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// �ҋ@�X�e�[�g����A�������Ƃɑ��̃X�e�[�g�ֈڍs����
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
		// �������
		if (toggleWalk && (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE))
		{
			return PLAYER_STATE::STATE_WALK;
		}

		// ���ȏ�̓��͒l�ŏ������Ԃֈڍs
		if (inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_JOG;
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

void PlayerState_Idle::EnterState(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_IDLE), m_animSpeed * _deltaTime);
}

