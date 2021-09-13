#include "MoveComponentHuman.h"
#include "Math.h"
#include "InputController.h"
#include "Input.h"
#include "Mouse.h"
#include "PlayerHuman.h"
#include "PlayerManager.h"
#include "ThirdPersonCamera.h"

const float MoveComponentHuman::PLAYER_SPEED = 45.0f;
const float MoveComponentHuman::SPEED_WALK = 45.0f;
const float MoveComponentHuman::SPEED_JOG = 80.0f;
const float MoveComponentHuman::SPEED_RUN = 120.0f;


// �R���X�g���N�^
MoveComponentHuman::MoveComponentHuman(Actor* in_owner)
	:MoveComponent(in_owner)
	,m_toggleRun(false)
	,m_velocity(SPEED_WALK)
{
	
}

// �f�X�g���N�^
MoveComponentHuman::~MoveComponentHuman()
{
}

// �X�V����
void MoveComponentHuman::Update(float in_deltaTime)
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
void MoveComponentHuman::MovementByController(float in_deltaTime)
{
	// ���X�e�B�b�N���͒l�̎擾
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	//std::cout << "StickL::x = " << axisL.x << "::  ::y = " << axisL.y << std::endl;
	float axisLength = axisL.LengthSq();
	// ����臒l
	float axisThreshold = 0.01f;

	// �����Ԏ��A����臒l�����ȉ��ɂȂ����瑖��g�O������
	if (m_toggleRun && axisLength < axisThreshold && axisLength > -axisThreshold)
	{
		m_toggleRun = false;
	}
	// �u����v�{�^����������Ă��邩���擾
	// �ʏ��LB�{�^��
	if (!m_toggleRun)
	{
		m_toggleRun = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	}


	
	// �u����v�{�^����������Ă������A�����ɍ��X�e�B�b�N���͂�����Ă����ꍇ
	if (m_toggleRun && CONTROLLER_INSTANCE.GetIsInputAxisL())
	{
		m_velocity = SPEED_RUN;
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
void MoveComponentHuman::MovementByKeyboard(float in_deltaTime)
{
	// �L�[���͒l
	Vector2 inputAxis = Vector2::Zero;

	// �L�[����WASD�ɂ��ړ�����
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
	{
		inputAxis.x += 1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S))
	{
		inputAxis.x += -1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A))
	{
		inputAxis.y += -1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D))
	{
		inputAxis.y += 1.0f;
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
	resultPos += moveVec * PLAYER_SPEED * in_deltaTime;

	// �ŏI�I�ȃv���C���[�̍��W���Z�b�g
	m_owner->SetPosition(resultPos);
}
