#include "PlayerState_Run.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"


PlayerState_Run::PlayerState_Run()
{
	m_animSpeed = 15.0f;
}

PlayerState_Run::~PlayerState_Run()
{
}

PLAYER_STATE PlayerState_Run::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// ����X�e�[�g����A�������Ƃɑ��̃X�e�[�g�ֈڍs����
	//---------------------------------------------------------------------+

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

		// �����Ԃ̎��̂݁A�p�b�h�̓��͒l��臒l����������瑖�������
		if (inputVal < WALK_SPEED_LINE && inputVal > -WALK_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_IDLE;
		}

		std::cout << "ToggleRun = " << (int)m_toggleRun << std::endl;

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
		bool isIdle = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_UP) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_DOWN) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_RIGHT) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_LEFT);

		// �����ꂩ�̈ړ��L�[������+���V�t�g��������Ă�����A�����Ԃֈڍs
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_LSHIFT) && !isIdle)
		{
			return PLAYER_STATE::STATE_RUN;
		}
		// �����ꂩ�̈ړ��L�[��������Ă�����A������Ԃ�
		else if (!isIdle)
		{
			return PLAYER_STATE::STATE_WALK;
		}
	}

	return PLAYER_STATE::STATE_RUN;
}

void PlayerState_Run::EnterState(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_RUN), m_animSpeed * _deltaTime);
}
