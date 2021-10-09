#include "PlayerMovement.h"
#include "Math.h"
#include "InputController.h"
#include "Input.h"
#include "Mouse.h"
#include "Player.h"
#include "ThirdPersonCamera.h"

const float PlayerMovement::PLAYER_SPEED = 45.0f;
const float PlayerMovement::SPEED_WALK = 45.0f;
const float PlayerMovement::SPEED_JOG = 80.0f;
const float PlayerMovement::SPEED_SPRINT = 160.0f;

// �R���X�g���N�^
PlayerMovement::PlayerMovement(Player* _player)
	:MoveComponent(_player)
	,m_player(_player)
	,m_velocity(SPEED_WALK)
{
	
}

// �f�X�g���N�^
PlayerMovement::~PlayerMovement()
{
}

// �X�V����
void PlayerMovement::Update(float in_deltaTime)
{
	if (m_isActive)
	{
		if (m_padIsActive)
		{
			MovementByController(in_deltaTime);
		}
		else
		{
			MovementByKeyboard(in_deltaTime);
		}
	}

	// �d�͏���
	//m_owner->SetPosition(m_owner->GetPosition() - Vector3(0.0f, 0.0f, 2.0f));
}


// �v���C���[�̈ړ�����(�R���g���[��)
void PlayerMovement::MovementByController(float in_deltaTime)
{
	// ���X�e�B�b�N���͒l�̎擾
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	//std::cout << "StickL::x = " << axisL.x << "::  ::y = " << axisL.y << std::endl;
	float axisLength = axisL.LengthSq();
	// ����臒l
	float axisThreshold = 0.01f;

	// �v���C���[��Ԃ̎擾
	bool toggleSprint = m_player->GetToggleSprint();
	bool toggleCrouch = m_player->GetToggleCrouch();
	//-------------------------------------------------------------------------------+
	// �uSprint�v�g�O������
	//-------------------------------------------------------------------------------+
	// �ʏ��LB�{�^���ő���ֈڍs/���Ⴊ�ݏ�Ԃ̎��͖���
	if (!toggleSprint && !toggleCrouch)
	{
		toggleSprint = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

		// true�Ȃ瑖���Ԃ�
		// �ړ����x�𑖂�ɐݒ�
		if (toggleSprint)
		{
			m_velocity = SPEED_SPRINT;
			m_player->SetToggleSprint(true);
		}
		
	}
	// �����Ԏ��A����臒l�����ȉ��ɂȂ����瑖��g�O������
	if (toggleSprint && axisLength < axisThreshold && axisLength > -axisThreshold)
	{
		toggleSprint = false;
		m_player->SetToggleSprint(toggleSprint);
	}

	//--------------------------------------------------------------------------------------+
    // �uCrouch�v�g�O������
    //--------------------------------------------------------------------------------------+
	// ���Ⴊ�ݏ�Ԃ̎���Ctrl�ŉ���
	if (toggleCrouch)
	{
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_RIGHTSTICK))
		{
			m_player->SetToggleCrouch(false);
		}
	}
	// �E�X�e�B�b�N�������݂�crouch��Ԃ�/�����ԉ���
	if (!toggleCrouch)
	{
		toggleCrouch = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_RIGHTSTICK);

		// �ړ����x������ɐݒ�
		if (toggleCrouch)
		{
			m_velocity = SPEED_WALK;
			m_player->SetToggleSprint(false);
			m_player->SetToggleCrouch(true);
		}
		
	}

	//-------------------------------------------------------------------------------+
    // �ړ����x�X�V
    //-------------------------------------------------------------------------------+
	// �ʏ�ړ����x�ɃZ�b�g
	if(!toggleSprint && !toggleCrouch && (axisLength >= 0.55f || axisLength <= -0.55f))
	{
		m_velocity = SPEED_JOG;
	}



	// �v���C���[�̑O�i�E�E�����x�N�g����` (�J�����)
	//Vector3 charaForwardVec = m_playerHuman->GetForward();
	Vector3 forwardVec = GAME_INSTANCE.GetViewTarget() - GAME_INSTANCE.GetViewPos();
	forwardVec.z = 0.0f;
	forwardVec.Normalize();
	Vector3 charaRightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// �E�����x�N�g������J������]�p���Z�o
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, charaRightVec));

	// �E��]������]���H
	tmpVec = Vector3::Cross(Vector3::UnitX, charaRightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;
	Vector3 charaForwardVec = m_owner->GetForward(); // �L�����̑O�i�x�N�g��

	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	axisL = Vector2::Transform(axisL, rot);


	// �O�i�x�N�g���ƉE�����x�N�g������ړ��ʂ��Z�o
	Vector3 moveVec = Vector3::Zero;
	moveVec += Vector3(axisL.x, axisL.y, 0.0f);


	// ���̓L�[�̑��a
	if (moveVec.LengthSq() > axisThreshold)
	{
		// �����L�[����
		//charaForwardVec = moveVec;
		moveVec.Normalize();

		// �i�s�����Ɍ����ĉ�](���[�v����)
		if (charaForwardVec.y - moveVec.y >= 1.01f)
		{
			moveVec.x = 1.0f;
			moveVec.y = 0.0f;
		}
		if (charaForwardVec.y - moveVec.y <= -1.01f)
		{
			moveVec.x = 1.0f;
			moveVec.y = 0.0f;
		}
		moveVec = Vector3::Lerp(charaForwardVec, moveVec, 0.35f);

		printf("chara = x:%f | y:%f | z:%f\n", charaForwardVec.x, charaForwardVec.y, charaForwardVec.z);
		printf("moveV = x:%f | y:%f | z:%f\n", moveVec.x, moveVec.y, moveVec.z);

		m_owner->RotateToNewForward(moveVec);
	}


	// �v���C���[�̌��݈ʒu����A��`���������֑��x�������Z
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += moveVec * m_velocity * in_deltaTime;

	// �ŏI�I�ȃv���C���[�̍��W���Z�b�g
	m_owner->SetPosition(resultPos);
}

// �v���C���[�̈ړ�����(�L�[�{�[�h)
void PlayerMovement::MovementByKeyboard(float in_deltaTime)
{
	// �L�[���͒l
	Vector2 inputAxis = Vector2::Zero;
	// �ړ��L�[�̓��͏�Ԃ��擾
	bool pressW = INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W);
	bool pressA = INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A);
	bool pressS = INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S);
	bool pressD = INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D);


	bool toggleWalk = m_player->GetToggleWalk();
	bool toggleSprint = m_player->GetToggleSprint();
	bool toggleCrouch = m_player->GetToggleCrouch();
	//-------------------------------------------------------------------------------+
    // �uSprint�v�g�O������
    //-------------------------------------------------------------------------------+
	// �����Ԏ��A���͂���Ă��Ȃ������瑖��g�O������
	if (toggleSprint && !(pressW || pressA || pressS || pressD))
	{
		m_player->SetToggleSprint(false);
	}
	// �u����v�{�^��=��SHIFT��������Ă��邩���擾
	// ���Ⴊ�ݏ�Ԏ��͑���ֈڍs���Ȃ�
	// �ړ����x�𑖂�ɐݒ�
	if (!toggleSprint && !toggleCrouch)
	{
		toggleSprint = INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_LSHIFT);

		if (toggleSprint)
		{
			m_velocity = SPEED_SPRINT;
			m_player->SetToggleSprint(toggleSprint);
		}
	}

	//-------------------------------------------------------------------------------+
    // �uWalk�v�g�O������
    //-------------------------------------------------------------------------------+
	// ������Ԃ̃g�O���X�V
	// X�L�[�Ő؂�ւ�
	bool key = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_X);
	if (key)
	{
		if (!toggleWalk)
		{
			toggleWalk = true;
			m_player->SetToggleWalk(toggleWalk);
		}
		else
		{
			toggleWalk = false;
			m_player->SetToggleWalk(toggleWalk);
		}
		
	}

	//--------------------------------------------------------------------------------------+
    // �uCrouch�v�g�O������
    //--------------------------------------------------------------------------------------+
	// ���Ⴊ�ݏ�Ԃ̎���Ctrl�ŉ���
	if (toggleCrouch)
	{
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LCTRL))
		{
			m_player->SetToggleCrouch(false);
		}
	}
	// ��Ctrl�����ł��Ⴊ�ݏ�Ԃֈڍs
	if (!toggleCrouch)
	{
		toggleCrouch = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LCTRL);

		// �����Ԃ�����
		// �ړ����x������ɐݒ�
		if (toggleCrouch)
		{
			m_velocity = SPEED_WALK;
			m_player->SetToggleSprint(false);
			m_player->SetToggleCrouch(true);
		}

	}

	//-------------------------------------------------------------------------------+
    // �ړ����x�X�V
    //-------------------------------------------------------------------------------+
	// ������ԂłȂ��A���Ⴊ�ݏ�ԂłȂ��A�����ꂩ�Ɉړ����Ă���ꍇ
	// �ړ����x���W���O(������)�ɐݒ�
	if(!toggleSprint && !toggleCrouch && !(pressW || pressA || pressS || pressD))
	{
		m_velocity = SPEED_JOG;
	}


	// �L�[����WASD�ɂ��ړ�����
	if (pressW)
	{
		inputAxis.y -= 1.0f;
	}
	if (pressS)
	{
		inputAxis.y += 1.0f;
	}
	if (pressA)
	{
		inputAxis.x -= 1.0f;
	}
	if (pressD)
	{
		inputAxis.x += 1.0f;
	}

	// �v���C���[�̑O�i�E�E�����x�N�g����` (�J�����)
	Vector3 forwardVec = GAME_INSTANCE.GetViewTarget() - GAME_INSTANCE.GetViewPos();
	forwardVec.z = 0.0f;
	forwardVec.Normalize();
	Vector3 charaRightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// �E�����x�N�g������J������]�p���Z�o
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, charaRightVec));

	// �E��]������]���H
	tmpVec = Vector3::Cross(Vector3::UnitX, charaRightVec);
	angleSign = (tmpVec.z > 0.0f) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	inputAxis = Vector2::Transform(inputAxis, rot);

	// �O�i�x�N�g���ƉE�����x�N�g������ړ��ʂ��Z�o
	Vector3 moveVec = Vector3::Zero;
	moveVec = Vector3(inputAxis.x, inputAxis.y, 0.0f);

	// �v���C���[�̕���o����Ԃ̎擾
	bool toggleWeaponOut = m_player->GetToggleWeaponOut();

	// ���̓L�[�̑��a�����ȏ�ł���΁A�L��������͕����ɉ�]������
	// ����o����Ԃ̂Ƃ��̓L�����̌������J�����ɌŒ肳���邽�ߏ����𕪊�
	if (!toggleWeaponOut && (moveVec.LengthSq() >= 0.5f))
	{

		// �L�����̑O�i�x�N�g��
	    Vector3 charaForwardVec = m_owner->GetForward();
		//charaForwardVec.Normalize();

		// �����L�[����
		//charaForwardVec = moveVec;
	    moveVec.Normalize();

		// �i�s�����Ɍ����ĉ�](Lerp����)
		if (charaForwardVec.y - moveVec.y >= 1.01f)
		{
			moveVec.x = 1.0f;
			moveVec.y = 0.0f;
		}
		if (charaForwardVec.y - moveVec.y <= -1.01f)
		{
			moveVec.x = 1.0f;
			moveVec.y = 0.0f;
		}
		charaForwardVec = Vector3::Lerp(charaForwardVec, moveVec, 0.51f);

		printf("chara = x:%f | y:%f | z:%f\n", charaForwardVec.x, charaForwardVec.y, charaForwardVec.z);
		printf("moveV = x:%f | y:%f | z:%f\n", moveVec.x, moveVec.y, moveVec.z);

		// rotation���X�V
		m_owner->RotateToNewForward(charaForwardVec);
	}
	else if (toggleWeaponOut)
	{

		// �L�����̑O�i�x�N�g�����J�����Ɠ���
		Vector3 charaForwardVec = m_owner->GetForward();
		
		charaForwardVec = Vector3::Lerp(charaForwardVec, forwardVec, 0.51f);

		// rotation���X�V
		m_owner->RotateToNewForward(charaForwardVec);
	}
	

	// �v���C���[�̌��݈ʒu����A��`���������֑��x�������Z
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += moveVec * m_velocity * in_deltaTime;

	// �ŏI�I�ȃv���C���[�̍��W���Z�b�g
	m_owner->SetPosition(resultPos);
}
