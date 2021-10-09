#include "Player.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerState_Idle.h"
#include "PlayerState_Walk.h"
#include "PlayerState_Jog.h"
#include "PlayerState_Sprint.h"
#include "PlayerState_Crouch.h"
#include "PlayerState_CrouchMove.h"
#include "PlayerState_JumpStart.h"
#include "PlayerState_JumpFall.h"
#include "PlayerState_JumpLand.h"
#include "PlayerState_WeaponOut_Idle.h"
#include "PlayerState_WeaponOut_MoveForward.h"
#include "PlayerState_WeaponOut_MoveBack.h"
#include "PlayerState_WeaponOut_MoveLeft.h"
#include "PlayerState_WeaponOut_MoveRight.h"
#include "TPSCamera.h"
#include "PlayerMovement.h"
#include "PointLight.h"

const std::string Player::PLAYER_MESH_PATH      = "Data/Meshes/Actors/HumanRace/Player/Player_gonzales.gpmesh";
const std::string Player::PLAYER_SKEL_PATH      = "Data/Meshes/Actors/HumanRace/Player/Player_gonzales.gpskel";
const std::string Player::ANIM_IDLE_PATH        = "Data/Animation/Player/Player_Idle.gpanim";
const std::string Player::ANIM_WALK_PATH        = "Data/Animation/Player/Player_Walk.gpanim";
const std::string Player::ANIM_JOG_PATH         = "Data/Animation/Player/Player_Jog.gpanim";
const std::string Player::ANIM_RUN_PATH         = "Data/Animation/Player/Player_Run.gpanim";
const std::string Player::ANIM_CROUCH_PATH      = "Data/Animation/Player/Player_Crouch.gpanim";
const std::string Player::ANIM_CROUCH_MOVE_PATH = "Data/Animation/Player/Player_CrouchMove.gpanim";
const std::string Player::ANIM_JUMP_START_PATH  = "Data/Animation/Player/Player_JumpStart.gpanim";
const std::string Player::ANIM_JUMP_FALL_PATH   = "Data/Animation/Player/Player_JumpFall.gpanim";
const std::string Player::ANIM_JUMP_LAND_PATH   = "Data/Animation/Player/Player_JumpLanding.gpanim";

const std::string Player::ANIM_WEAPOUT_IDLE_PATH       = "Data/Animation/Player/Player_WeapOut_Idle_1.gpanim";
const std::string Player::ANIM_WEAPOUT_MOVE_FWD_PATH   = "Data/Animation/Player/Player_WeapOut_Move_Forward.gpanim";
const std::string Player::ANIM_WEAPOUT_MOVE_BWD_PATH   = "Data/Animation/Player/Player_WeapOut_Move_Back.gpanim";
const std::string Player::ANIM_WEAPOUT_MOVE_RIGHT_PATH = "Data/Animation/Player/Player_WeapOut_Move_Right.gpanim";
const std::string Player::ANIM_WEAPOUT_MOVE_LEFT_PATH  = "Data/Animation/Player/Player_WeapOut_Move_Left.gpanim";

const std::string Player::ANIM_WEAPOUT_WALK_FWD_PATH = "Data/Animation/Player/Player_WeapOut_Walk_Forward.gpanim";
const std::string Player::ANIM_WEAPOUT_WALK_BWD_PATH = "Data/Animation/Player/Player_WeapOut_Walk_Back.gpanim";
const std::string Player::ANIM_WEAPOUT_WALK_RIGHT_PATH = "Data/Animation/Player/Player_WeapOut_Walk_Right.gpanim";
const std::string Player::ANIM_WEAPOUT_WALK_LEFT_PATH = "Data/Animation/Player/Player_WeapOut_Walk_Left.gpanim";

Player::Player()
	:Actor(OBJECT_TAG::PLAYER)
	,m_nowState(PLAYER_STATE::STATE_IDLE)
	,m_nextState(PLAYER_STATE::STATE_IDLE)
	,m_tpsCamera(nullptr)
	,m_light(nullptr)
	,m_toggleSprint(false)
	,m_toggleWalk(false)
	,m_toggleCrouch(false)
	,m_toggleWeaponOut(false)
	,m_isWeaponOutChange(false)
	,m_weaponOutPressStart(0)
	,m_weaponOutPressCount(0)
{

	// �J�����̐���(�O�l�̃J����)
	m_tpsCamera = new TPSCamera(this);
	m_tpsCamera->SetCameraLength(Vector3(120.0f, 120.0f, 120.0f));
	m_tpsCamera->SetAdjustTargetPos(Vector2(-40.0f, -105.0f));

	// �ړ��R���|�[�l���g�̒ǉ�
	PlayerMovement* moveComp = new PlayerMovement(this);

	// ���C�g�̐���
	m_light = new PointLight(PointLight::VL_BIG);
	m_light->SetPosition(m_position);
	m_light->SetLightColor(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
	m_light->SetLuminance(5.0f);

	// �X�P���^�����b�V���̓ǂݍ���
	Mesh* mesh = RENDERER->GetMesh(PLAYER_MESH_PATH);
	m_skelComp = new SkeletalMeshComponent(this);
	m_skelComp->SetMesh(mesh);
	m_skelComp->SetSkeleton(RENDERER->GetSkeleton(PLAYER_SKEL_PATH));

	// �A�j���[�V�����̃��[�h
	m_anims.resize(static_cast<unsigned int>(PLAYER_STATE::STATE_ALL_NUM));
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_IDLE)]   = RENDERER->GetAnimation(ANIM_IDLE_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WALK)]   = RENDERER->GetAnimation(ANIM_WALK_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_JOG)]    = RENDERER->GetAnimation(ANIM_JOG_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_SPRINT)] = RENDERER->GetAnimation(ANIM_RUN_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_CROUCH)] = RENDERER->GetAnimation(ANIM_CROUCH_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_CROUCH_MOVE)] = RENDERER->GetAnimation(ANIM_CROUCH_MOVE_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_JUMP_START)] = RENDERER->GetAnimation(ANIM_JUMP_START_PATH.c_str(), false);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_JUMP_FALL)] = RENDERER->GetAnimation(ANIM_JUMP_FALL_PATH.c_str(), false);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_JUMP_LAND)] = RENDERER->GetAnimation(ANIM_JUMP_LAND_PATH.c_str(), false);
	
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_IDLE)]      = RENDERER->GetAnimation(ANIM_WEAPOUT_IDLE_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD)] = RENDERER->GetAnimation(ANIM_WEAPOUT_MOVE_FWD_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_MOVEBWD)] = RENDERER->GetAnimation(ANIM_WEAPOUT_MOVE_BWD_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_MOVERIGHT)] = RENDERER->GetAnimation(ANIM_WEAPOUT_MOVE_RIGHT_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_MOVELEFT)] = RENDERER->GetAnimation(ANIM_WEAPOUT_MOVE_LEFT_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_WALKFWD)] = RENDERER->GetAnimation(ANIM_WEAPOUT_WALK_FWD_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_WALKBWD)] = RENDERER->GetAnimation(ANIM_WEAPOUT_WALK_BWD_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_WALKRIGHT)] = RENDERER->GetAnimation(ANIM_WEAPOUT_WALK_RIGHT_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WEAPONOUT_WALKLEFT)] = RENDERER->GetAnimation(ANIM_WEAPOUT_WALK_LEFT_PATH.c_str(), true);

	// �v���C���[�X�e�[�g�v�[���̐���
	m_statePool.push_back(new PlayerState_Idle);
	m_statePool.push_back(new PlayerState_Walk);
	m_statePool.push_back(new PlayerState_Jog);
	m_statePool.push_back(new PlayerState_Sprint);
	m_statePool.push_back(new PlayerState_Crouch);
	m_statePool.push_back(new PlayerState_CrouchMove);
	m_statePool.push_back(new PlayerState_JumpStart);
	m_statePool.push_back(new PlayerState_JumpFall);
	m_statePool.push_back(new PlayerState_JumpLand);

	m_statePool.push_back(new PlayerState_WeaponOut_Idle);
	m_statePool.push_back(new PlayerState_WeaponOut_MoveForward);
	m_statePool.push_back(new PlayerState_WeaponOut_MoveBack);
	m_statePool.push_back(new PlayerState_WeaponOut_MoveLeft);
	m_statePool.push_back(new PlayerState_WeaponOut_MoveRight);

	// �ҋ@��Ԃ��J�n
	m_statePool[static_cast<unsigned int>(m_nowState)]->EnterState(this);

}

Player::~Player()
{
}

void Player::UpdateActor(float _deltaTime)
{
	// ���C�g�Ǐ]
	m_light->SetPosition(m_position);

	// �v���C���[�X�e�[�g�̍X�V
	UpdatePlayerState(_deltaTime);
}

/// <summary>
/// ����̎��o����Ԃ̍X�V
/// </summary>
void Player::UpdateWeaponOut()
{
	// ����o����ԂłȂ����̍X�V
	if (!m_toggleWeaponOut && !m_isWeaponOutChange)
	{
		// R�L�[��X�{�^���ŕ���o���g�O��ON
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_R) || CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_X))
		{
			m_toggleWeaponOut = true;
			m_isWeaponOutChange = true;

			// �J����������ڋ�
			//m_tpsCamera->SetCameraLength(Vector3(50.0f, 50.0f, 50.0f));

			printf("������o����\n");
		}
	}

	// ����o����Ԃ̎��̍X�V
	if (m_toggleWeaponOut && !m_isWeaponOutChange)
	{
		// R�L�[��X�{�^���������񂾎��ɒ������J�E���g�J�n
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_R) || CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_X))
		{
			m_weaponOutPressStart = SDL_GetTicks() / 1000;
			printf("������߃J�E���g�J�n\n");
		}

		// R�L�[��X�{�^�������Ă�ԃJ�E���g�X�V
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_R) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_X))
		{
			m_weaponOutPressCount = SDL_GetTicks() / 1000;
		}

		// �{�^���������ꂽ��J�E���g�����Z�b�g
		if(INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_R) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X))
		{
			m_weaponOutPressStart = 0;
			m_weaponOutPressCount = 0;
		}

		// 0.5�b�������ŕ���o���g�O��OFF
		if (m_weaponOutPressStart - m_weaponOutPressCount > 500)
		{
			m_toggleWeaponOut = false;
			m_isWeaponOutChange = true;
			m_weaponOutPressStart = 0;
			m_weaponOutPressCount = 0;

			printf("����o��OFF\n");
		}
	}

	// ����o���̐؂�ւ��t���O�X�V
	// �{�^���𗣂�����
	if (m_isWeaponOutChange)
	{
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_R) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X))
		{
			m_isWeaponOutChange = false;
		}
	}

}

/// <summary>
/// �v���C���[�X�e�[�g�̍X�V
/// </summary>
/// <param name="_deltaTime"> �f���^�^�C�� </param>
void Player::UpdatePlayerState(float _deltaTime)
{
	// ����o����Ԃ̃g�O���X�V
	UpdateWeaponOut();

	// �O������̃X�e�[�g�ύX�������������`�F�b�N
	if (m_nowState != m_nextState)
	{
		m_statePool[static_cast<unsigned int>(m_nextState)]->EnterState(this);
		m_nowState = m_nextState;
		return;
	}

	// �X�e�[�g�X�V
	m_nextState = m_statePool[static_cast<unsigned int>(m_nowState)]->Update(this, _deltaTime);

	// ���݃X�e�[�g�̍X�V�ɂ���āA�X�e�[�g�ύX�������������`�F�b�N
	if (m_nowState != m_nextState)
	{
		m_statePool[static_cast<unsigned int>(m_nextState)]->EnterState(this);
		m_nowState = m_nextState;
	}
}
