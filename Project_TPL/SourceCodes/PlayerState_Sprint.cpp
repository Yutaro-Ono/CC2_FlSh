#include "PlayerState_Sprint.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"


PlayerState_Sprint::PlayerState_Sprint()
{
	m_animSpeed = 0.5f;
}

PlayerState_Sprint::~PlayerState_Sprint()
{
}

PLAYER_STATE PlayerState_Sprint::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// ����X�e�[�g����A�������Ƃɑ��̃X�e�[�g�ֈڍs����
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

		// ���Ⴊ�ݏ��
		if (toggleCrouch)
		{
			return PLAYER_STATE::STATE_CROUCH;
		}

		// �����Ԃ̎��̂݁A�p�b�h�̓��͒l��臒l����������瑖�������
		if (toggleSprint && inputVal < WALK_SPEED_LINE && inputVal > -WALK_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_IDLE;
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

		// ���Ⴊ�ݏ��
		if (toggleCrouch)
		{
			return PLAYER_STATE::STATE_CROUCH;
		}

		// �����ꂩ�̈ړ��L�[������+�����ԂŁA����p��
		if (toggleSprint && !isIdle)
		{
			return PLAYER_STATE::STATE_SPRINT;
		}
		// �����ꂩ�̈ړ��L�[��������Ă����A�����g�O���������Ă�����A������Ԃ�
		if (!isIdle && toggleWalk)
		{
			return PLAYER_STATE::STATE_WALK;
		}
		// �ړ��L�[��������Ă��邪�����g�O���������Ă��Ȃ��ꍇ�́A�������Ԃ�
		if (!isIdle)
		{
			return PLAYER_STATE::STATE_JOG;
		}
		// ����������Ă��Ȃ�������ҋ@���
		if (isIdle)
		{
			return PLAYER_STATE::STATE_IDLE;
		}

	}

	return PLAYER_STATE::STATE_SPRINT;
}

void PlayerState_Sprint::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_SPRINT), m_animSpeed);
}
