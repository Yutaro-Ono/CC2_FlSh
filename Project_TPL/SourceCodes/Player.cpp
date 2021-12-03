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
#include "PlayerState_Aim.h"
#include "TPSCamera.h"
#include "FirstPersonCameraComponent.h"
#include "PlayerMovement.h"
#include "PointLight.h"
#include "BoxColliderComponent.h"

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

const std::string Player::ANIM_AIM_STAND = "Data/Animation/Player/Player_ToAim_Stand_NoLoop.gpanim";
const std::string Player::ANIM_AIM_CROUCH = "Data/Animation/Player/Player_ToAim_Crouch_NoLoop.gpanim";

Player::Player()
	:Actor(OBJECT_TAG::ACTOR_PLAYER)
	,m_nowState(PLAYER_STATE::STATE_IDLE)
	,m_nextState(PLAYER_STATE::STATE_IDLE)
	,m_tpsCamera(nullptr)
	,m_light(nullptr)
	,m_boxCollider(nullptr)
	,m_toggleSprint(false)
	,m_toggleWalk(false)
	,m_toggleCrouch(false)
	,m_toggleWeaponOut(false)
	,m_isWeaponOutChange(false)
	,m_weaponOutPressStart(0)
	,m_weaponOutPressCount(0)
{


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
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_IDLE)] = RENDERER->GetAnimation(ANIM_IDLE_PATH.c_str(), true);
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

	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_AIM_STAND)] = RENDERER->GetAnimation(ANIM_AIM_STAND.c_str(), false);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_AIM_CROUCH)] = RENDERER->GetAnimation(ANIM_AIM_CROUCH.c_str(), false);


	// �v���C���[�X�e�[�g�v�[���̐���
	m_statePool.push_back(new PlayerState_Idle);
	m_statePool.push_back(new PlayerState_Walk);
	m_statePool.push_back(new PlayerState_Jog);
	m_statePool.push_back(new PlayerState_Sprint);
	m_statePool.push_back(new PlayerState_Crouch);
	m_statePool.push_back(new PlayerState_CrouchMove);
	m_statePool.push_back(new PlayerState_Aim);

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

	// �J�����̐���(��l�̃J����)
	m_fpsCamera = new FirstPersonCameraComponent(this);
	// �J�����̐���(�O�l�̃J����)
	m_tpsCamera = new TPSCamera(this);
	m_tpsCamera->SetCameraLength(Vector3(160.0f, 160.0f, 160.0f));
	//m_tpsCamera->SetCameraLength(Vector3(260.0f, 260.0f, 260.0f));
	m_tpsCamera->SetAdjustTargetPos(Vector2(-50.0f, -105.0f));
	m_tpsCamera->SetActive();

	// �����蔻��(�{�b�N�X)
	AABB box = mesh->GetCollisionBox();
	box.m_isRotatable = false;
	m_boxCollider = new BoxColliderComponent(this);
	m_boxCollider->SetObjectBox(box);

	// �ړ��R���|�[�l���g�̒ǉ�
	PlayerMovement* moveComp = new PlayerMovement(this);
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
			m_weaponOutPressStart = SDL_GetTicks();
			printf("������߃J�E���g�J�n\n");
		}

		// R�L�[��X�{�^�������Ă�ԃJ�E���g�X�V
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_R) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_X))
		{
			m_weaponOutPressCount = SDL_GetTicks();
		}

		// �{�^���������ꂽ��J�E���g�����Z�b�g
		if(INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_R) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X))
		{
			m_weaponOutPressStart = 0;
			m_weaponOutPressCount = 0;
		}

		// 1�b�������ŕ���o���g�O��OFF
		if (m_weaponOutPressCount > m_weaponOutPressStart + 600)
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
/// �����蔻�菈��
/// </summary>
/// <param name="_ownCollComp"> ���̃N���X�̓����蔻��R���|�[�l���g </param>
/// <param name="_otherCollComp"> �Փ˂�������̓����蔻��R���|�[�l���g </param>
void Player::OnCollisionEnter(ColliderComponent* _ownCollComp, ColliderComponent* _otherCollComp)
{
	// �^�O���Ƃɏ����𕪊�
	OBJECT_TAG otherTag = _otherCollComp->GetOwnerTag();

	// ���I�u�W�F�N�g�Ƃ̓����蔻��
	if (otherTag == OBJECT_TAG::STATIC_OBJECT)
	{
		if (_otherCollComp->GetColliderType() == COLLIDER_TYPE::TYPE_BOX)
		{
			Vector3 fix;

			//�ǂƂԂ������Ƃ�
			AABB bgBox = dynamic_cast<BoxColliderComponent*>(_otherCollComp)->GetWorldBox();
			AABB playerBox = m_boxCollider->GetWorldBox();

			// �߂荞�݂��C��
			CalcCollisionFixVec(playerBox, bgBox, fix);

			// �␳�x�N�g�����߂�
			m_position += fix;
			// �ʒu�Čv�Z
			//ComputeWorldTransform();
		}

	}

	// �G�Ƃ̓����蔻��
	if (otherTag == OBJECT_TAG::ACTOR_ENEMY)
	{
		if (_otherCollComp->GetColliderType() == COLLIDER_TYPE::TYPE_BOX)
		{
			Vector3 fix;

			//�ǂƂԂ������Ƃ�
			AABB bgBox = dynamic_cast<BoxColliderComponent*>(_otherCollComp)->GetWorldBox();
			AABB playerBox = m_boxCollider->GetWorldBox();

			// �߂荞�݂��C��
			CalcCollisionFixVec(playerBox, bgBox, fix);

			// �␳�x�N�g�����߂�
			m_position += fix;
			// �ʒu�Čv�Z
			//ComputeWorldTransform();
		}
	}


}

/// <summary>
/// �G�C�����쎞:�O�l�̎��_�J��������l�̎��_�J�����ɐ؂�ւ��A�v���C���[���\���ɂ���
/// �G�C��������:��l�̎��_�J�������O�l�̎��_�J�����ɐ؂�ւ��A�v���C���[���ĕ\������
/// </summary>
/// <param name="_isAim"> �G�C����Ԃ��ǂ��� </param>
void Player::SetActiveAimCamera(bool _isAim)
{
	if (_isAim)
	{
		m_fpsCamera->SetActive();
		m_skelComp->SetVisible(false);

		return;
	}

	if (!_isAim)
	{
		m_tpsCamera->SetActive();
		m_tpsCamera->SetCameraLength(Vector3(160.0f, 160.0f, 160.0f));
		//m_tpsCamera->SetCameraLength(Vector3(260.0f, 260.0f, 260.0f));
		m_tpsCamera->SetAdjustTargetPos(Vector2(-50.0f, -105.0f));
		m_skelComp->SetVisible(true);

		return;
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
