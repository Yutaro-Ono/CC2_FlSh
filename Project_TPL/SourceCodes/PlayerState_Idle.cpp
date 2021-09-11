#include "PlayerState_Idle.h"
#include "GameMain.h"


PlayerStateIdle::PlayerStateIdle()
{
}

PlayerStateIdle::~PlayerStateIdle()
{
}

PLAYER_STATE PlayerStateIdle::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// �ҋ@�X�e�[�g����A�������Ƃɑ��̃X�e�[�g�ֈڍs����
	//---------------------------------------------------------------------+

	// �R���g���[���ڑ���
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �W�����v�����ւ̈ڍs
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_A))
		{
			return PLAYER_STATE::STATE_JUMP_START;
		}

		// �ҋ@��Ԃ�(�ړ��{�^����������Ă��邩)
		bool isIdle = CONTROLLER_INSTANCE.IsKeyOff(SDL_CONTROLLER) &
			          CONTROLLER_INSTANCE.IsKeyOff(SDL_CONTROLLER_) &
			          CONTROLLER_INSTANCE.IsKeyOff(SDL_CONTROLLER_) &
			          CONTROLLER_INSTANCE.IsKeyOff(SDL_CONTROLLER_);

		if (!isIdle && )
		{
			return PLAYER_STATE::STATE_WALK;
		}
	}

	else
	{
		// �W�����v�����ւ̈ڍs
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_SPACE))
		{
			return PLAYER_STATE::STATE_JUMP_START;
		}

		// �ҋ@��Ԃ�(�ړ��L�[��������Ă��邩)
		bool isIdle = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_UP) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_DOWN) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_RIGHT) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_LEFT);
	}




	return PLAYER_STATE::STATE_IDLE;
}

void PlayerStateIdle::EnterState(Player* _player, float _deltaTime)
{
}
