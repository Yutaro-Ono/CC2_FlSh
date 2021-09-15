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
const float PlayerMovement::SPEED_SPRINT = 120.0f;


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

	//-------------------------------------------------------------------------------+
	// �uSprint�v�g�O������
	//-------------------------------------------------------------------------------+
	// �u����v�{�^����������Ă��邩���擾
	// �ʏ��LB�{�^��
	bool toggleSprint = m_player->GetToggleSprint();
	if (!toggleSprint)
	{
		toggleSprint = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

		// true�Ȃ瑖���Ԃ�/���Ⴊ�ݏ�Ԃ�����
		if (toggleSprint)
		{
			m_player->SetToggleCrouch(false);
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
	bool toggleCrouch = m_player->GetToggleCrouch();
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

		if (toggleCrouch)
		{
			m_player->SetToggleSprint(false);
			m_player->SetToggleCrouch(true);
		}
		
	}

	//-------------------------------------------------------------------------------+
    // �ړ����x�X�V
    //-------------------------------------------------------------------------------+
	// �u����v�{�^����������Ă������A�����ɍ��X�e�B�b�N���͂�����Ă����ꍇ
	if (toggleSprint && CONTROLLER_INSTANCE.GetIsInputAxisL())
	{
		m_velocity = SPEED_SPRINT;
	}
	else if(axisLength >= 0.55f || axisLength <= -0.55f)
	{
		m_velocity = SPEED_JOG;
	}
	else
	{
		m_velocity = SPEED_WALK;
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
		charaForwardVec = moveVec;
		// �i�s�����Ɍ����ĉ�]
		charaForwardVec.Normalize();
		m_owner->RotateToNewForward(charaForwardVec);
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

	//-------------------------------------------------------------------------------+
    // �uSprint�v�g�O������
    //-------------------------------------------------------------------------------+
	// �����Ԏ��A���͂���Ă��Ȃ������瑖��g�O������
	bool toggleSprint = m_player->GetToggleSprint();
	if (toggleSprint && !(pressW || pressA || pressS || pressD))
	{
		toggleSprint = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LSHIFT);

		// true�Ȃ瑖���Ԃ�/���Ⴊ�ݏ�Ԃ�����
		if (toggleSprint)
		{
			m_player->SetToggleCrouch(false);
			m_player->SetToggleSprint(true);
		}
	}
	// �u����v�{�^����������Ă��邩���擾
	// ���V�t�g
	if (!toggleSprint)
	{
		toggleSprint = INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_LSHIFT);
		m_player->SetToggleSprint(toggleSprint);
	}

	//-------------------------------------------------------------------------------+
    // �uWalk�v�g�O������
    //-------------------------------------------------------------------------------+
	// ������Ԃ̃g�O���X�V
	// LeftAlt�ōX�V
	bool toggleWalk = m_player->GetToggleWalk();
	bool key = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LALT);
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
	bool toggleCrouch = m_player->GetToggleCrouch();
	// ���Ⴊ�ݏ�Ԃ̎���Ctrl�ŉ���
	if (toggleCrouch)
	{
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LCTRL))
		{
			m_player->SetToggleCrouch(false);
		}
	}
	// �E�X�e�B�b�N�������݂�crouch��Ԃ�/�����ԉ���
	if (!toggleCrouch)
	{
		toggleCrouch = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LCTRL);

		if (toggleCrouch)
		{
			m_player->SetToggleSprint(false);
			m_player->SetToggleCrouch(true);
		}

	}

	//-------------------------------------------------------------------------------+
    // �ړ����x�X�V
    //-------------------------------------------------------------------------------+
	// �u����v�{�^����������Ă������A�����ɍ��X�e�B�b�N���͂�����Ă����ꍇ
	if (toggleSprint)
	{
		m_velocity = SPEED_SPRINT;
	}
	else if (!toggleWalk)
	{
		m_velocity = SPEED_JOG;
	}
	else
	{
		m_velocity = SPEED_WALK;
	}


	// �L�[����WASD�ɂ��ړ�����
	if (pressW)
	{
		inputAxis.y += -1.0f;
	}
	if (pressS)
	{
		inputAxis.y += 1.0f;
	}
	if (pressA)
	{
		inputAxis.x += -1.0f;
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
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;
	Vector3 charaForwardVec = m_owner->GetForward(); // �L�����̑O�i�x�N�g��

	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	inputAxis = Vector2::Transform(inputAxis, rot);


	// �O�i�x�N�g���ƉE�����x�N�g������ړ��ʂ��Z�o
	Vector3 moveVec = Vector3::Zero;
	moveVec += Vector3(inputAxis.x, inputAxis.y, 0.0f);

	// ���̓L�[�̑��a
	if (moveVec.LengthSq() > 0.5f)
	{
		// �����L�[����
		charaForwardVec = moveVec;
		// �i�s�����Ɍ����ĉ�]
		charaForwardVec.Normalize();
		m_owner->RotateToNewForward(charaForwardVec);
	}


	// �v���C���[�̌��݈ʒu����A��`���������֑��x�������Z
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += moveVec * m_velocity * in_deltaTime;

	// �ŏI�I�ȃv���C���[�̍��W���Z�b�g
	m_owner->SetPosition(resultPos);
}
