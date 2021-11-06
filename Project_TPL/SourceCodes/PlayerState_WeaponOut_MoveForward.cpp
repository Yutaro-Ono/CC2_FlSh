#include "PlayerState_WeaponOut_MoveForward.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

// �e��ړ��A�j���[�V�����̑��x(���ӁF�����l�ɂ���Ƒ���A�j���[�V�������x�̍X�V�ɖ�肪������)
const float PlayerState_WeaponOut_MoveForward::MOVE_ANIM_SPEED = 0.35f;
const float PlayerState_WeaponOut_MoveForward::SPRINT_ANIM_SPEED = 0.48f;
const float PlayerState_WeaponOut_MoveForward::WALK_ANIM_SPEED = 0.65f;

PlayerState_WeaponOut_MoveForward::PlayerState_WeaponOut_MoveForward()
{
	m_animSpeed = 0.35f;
}

PlayerState_WeaponOut_MoveForward::~PlayerState_WeaponOut_MoveForward()
{
}

PLAYER_STATE PlayerState_WeaponOut_MoveForward::Update(Player* _player, float _deltaTime)
{
	// �E�N���b�N���A�G�C����Ԃ֑J��
	if (MOUSE_INSTANCE.GetButtonValue(Mouse::MouseButtonState::MOUSE_BUTTON_PRESSED))
	{
		_player->SetToggleSprint(false);
		m_animSpeed = MOVE_ANIM_SPEED;
		// �v���C���[�̑����Ԃɉ������A�j���[�V�����̑��x���X�V
		UpdateSprintAnimSpeed(_player);

		return PLAYER_STATE::STATE_AIM;
	}

	// �v���C���[�̑����Ԃɉ������A�j���[�V�����̑��x���X�V
	UpdateSprintAnimSpeed(_player);

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
			if (move.y >= JOG_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}

			// ���ȏ�̓��͒l+������͎��ɁA����ړ���Ԃֈڍs
			if (move.y <= -JOG_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEBWD;
			}

			// ���ȏ�̓��͒l+�����͎��ɁA�������ړ���Ԃֈڍs
			if (move.x <= -JOG_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVELEFT;
			}

			// ���ȏ�̓��͒l+�E���͎��ɁA�E�����ړ���Ԃֈڍs
			if (move.x >= JOG_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVERIGHT;
			}

			//-----------------------------------------------------------+
			// �����X�e�[�g
			//-----------------------------------------------------------+
			// ���ȏ�̓��͒l+�O�����͎��ɁA�O���ړ���Ԃֈڍs
			if (move.y >= WALK_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_WALKFWD;
			}

			// ���ȏ�̓��͒l+������͎��ɁA����ړ���Ԃֈڍs
			if (move.y <= -WALK_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_WALKBWD;
			}

			// ���ȏ�̓��͒l+�����͎��ɁA�������ړ���Ԃֈڍs
			if (move.x <= -WALK_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_WALKLEFT;
			}

			// ���ȏ�̓��͒l+�E���͎��ɁA�E�����ړ���Ԃֈڍs
			if (move.x >= WALK_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_WALKRIGHT;
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

	return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;

}

void PlayerState_WeaponOut_MoveForward::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	// ���s��Ԃ��ǂ����Ń��[�V������ύX
	if (_player->GetToggleWalk())
	{
		m_animSpeed = WALK_ANIM_SPEED;

		// ���s��Ԃ̎�
		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_WEAPONOUT_WALKFWD), m_animSpeed);
	}
	else
	{
		m_animSpeed = MOVE_ANIM_SPEED;

		// �v���C���[�������Ԃ̎��A�j���[�V�������x�𑁂߂�
		if (_player->GetToggleSprint())
		{
			m_animSpeed = SPRINT_ANIM_SPEED;
		}

		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD), m_animSpeed);
	}
}

/// <summary>
/// �v���C���[�̑����Ԃ��擾���A�����Ԃ�������A�j���[�V�����̑��x�𑁂߂čĐ�������
/// </summary>
void PlayerState_WeaponOut_MoveForward::UpdateSprintAnimSpeed(Player* _player)
{
	// �v���C���[�������Ԃ��A�j���[�V�������x�����葬�x�ɐݒ肳��Ă��Ȃ���
	// �A�j���[�V�������x�𑁂߂�
	if (_player->GetToggleSprint() && m_animSpeed != SPRINT_ANIM_SPEED)
	{
		m_animSpeed = SPRINT_ANIM_SPEED;

		SkeletalMeshComponent* skel = _player->GetSkelMesh();
		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD), m_animSpeed);
	}
}
