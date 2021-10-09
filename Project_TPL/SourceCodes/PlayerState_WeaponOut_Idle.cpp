#include "PlayerState_WeaponOut_Idle.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

PlayerState_WeaponOut_Idle::PlayerState_WeaponOut_Idle()
{
}

PlayerState_WeaponOut_Idle::~PlayerState_WeaponOut_Idle()
{
}

PLAYER_STATE PlayerState_WeaponOut_Idle::Update(Player* _player, float _deltaTime)
{
	// �o�C���h�����v���C���[�����Ԃ̎擾
	bool toggleWalk = _player->GetToggleWalk();              // ���s���
	bool toggleCrouch = _player->GetToggleCrouch();          // ���Ⴊ�ݏ��
	bool toggleWeaponOut = _player->GetToggleWeaponOut();    // ����o�����

	// �R���g���[���ڑ���
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �ҋ@��Ԃ�(�ړ��{�^����������Ă��邩)
		Vector2 move = CONTROLLER_INSTANCE.GetLAxisVec();
		float inputVal = move.LengthSq();

		// ����o�����
		if (toggleWeaponOut)
		{
			// ���Ⴊ�ݏ��
			if (toggleCrouch)
			{
				//return PLAYER_STATE::STATE_WEAPONOUT_CROUCH;
			}

			// ���ȏ�̓��͒l+�O�����͎��ɁA�O���ړ���Ԃֈڍs
			if ((inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE) && move.y > 0.0f)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}

			// ���ȏ�̓��͒l+������͎��ɁA����ړ���Ԃֈڍs
			if ((inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE) && move.y < 0.0f)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEBWD;
			}

			// �������
			if (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}

			// ���͂��Ȃ��ꍇ�͑ҋ@��Ԃ�
			return PLAYER_STATE::STATE_WEAPONOUT_IDLE;
		}
		else
		{
			// �W�����v�����ւ̈ڍs
			if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_Y))
			{
				//return PLAYER_STATE::STATE_JUMP_START;
			}

			// ���Ⴊ�ݏ��
			if (toggleCrouch)
			{
				//return PLAYER_STATE::STATE_WEAPONOUT_CROUCH;
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

			// ���͂��Ȃ��ꍇ�͑ҋ@��Ԃ�
			return PLAYER_STATE::STATE_IDLE;
		}
	}

	// �R���g���[�����ڑ���
	else
	{
		// ����o�����
		if (toggleWeaponOut)
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


			// �㉺���E�̓��͂ɂ����
			// �e�����̈ړ����[�V�����֑J��
			if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}
			// �����g�O���L������S�L�[��������Ă��������ړ���Ԃ�
			if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S))
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEBWD;
			}
			// �����g�O���L������A�L�[��������Ă����獶�ړ���Ԃ�
			if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A))
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVELEFT;
			}
			// �����g�O���L������D�L�[��������Ă�����E��Ԃ�
			if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D))
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVERIGHT;
			}


			// �ړ����Ă���ꍇ�A������
			if (!isIdle)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}

			// ���Ⴊ�ݏ��
			if (toggleCrouch)
			{
				return PLAYER_STATE::STATE_CROUCH;
			}
			// ���͂��Ȃ��ꍇ�͑ҋ@��Ԃ�
			return PLAYER_STATE::STATE_WEAPONOUT_IDLE;
		}

		// ������ߏ��
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

			// ���͂��Ȃ��ꍇ�͑ҋ@��Ԃ�
			return PLAYER_STATE::STATE_IDLE;
		}
	}
}

void PlayerState_WeaponOut_Idle::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_WEAPONOUT_IDLE), m_animSpeed);
}