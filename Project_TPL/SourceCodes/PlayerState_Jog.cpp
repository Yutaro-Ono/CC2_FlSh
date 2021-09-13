#include "PlayerState_Jog.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"


PlayerState_Jog::PlayerState_Jog()
{
	m_animSpeed = 15.0f;
}

PlayerState_Jog::~PlayerState_Jog()
{
}

PLAYER_STATE PlayerState_Jog::Update(Player* _player, float _deltaTime)
{

	//---------------------------------------------------------------------+
    // ������X�e�[�g����A�������Ƃɑ��̃X�e�[�g�ֈڍs����
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
		// �����ԏo�Ȃ��ꍇ�̂݁A�u����v�{�^���̏�Ԃ��擾
		if (!m_toggleRun)
		{
			m_toggleRun = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		}

		// �����Ԃ̎��̂݁A�p�b�h�̓��͒l��臒l����������瑖�������
		if (m_toggleRun && inputVal < WALK_SPEED_LINE && inputVal > -WALK_SPEED_LINE)
		{
			m_toggleRun = false;
		}

		std::cout << "ToggleRun = " << (int)m_toggleRun << std::endl;


		// ����t���O��true���uWALK�v�ȏ�̓��͒l�ő����Ԃֈڍs
		if (m_toggleRun && (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE))
		{
			return PLAYER_STATE::STATE_RUN;
		}

		// ���ȏ�̓��͒l�ŏ������Ԃֈڍs
		if (inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_JOG;
		}

		// ������̊�𖞂����Ă��Ȃ����̂́A���ȏ�̓��͒l������ꍇ
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

	m_toggleRun = false;
	return PLAYER_STATE::STATE_IDLE;
}

void PlayerState_Jog::EnterState(Player* _player, float _deltaTime)
{
	m_toggleRun = false;
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_JOG), m_animSpeed * _deltaTime);
}
