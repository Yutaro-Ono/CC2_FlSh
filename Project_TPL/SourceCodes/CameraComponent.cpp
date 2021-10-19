#include "CameraComponent.h"
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Actor.h"

CameraComponent::CameraComponent(Actor * _target, int _updateOrder)
	:Component(_target, _updateOrder)
	,m_cameraStatus(CAMERA_STATUS::DEACTIVE)
	,m_position(Vector3::Zero)
	,m_fov(75.0f)
	,m_near(1.0f)
	,m_far(40000.0f)
{
}

CameraComponent::~CameraComponent()
{
	// �Q�[�����炱�̃J�������폜
	GAME_INSTANCE.InActiveCamera(this);
}

// �v���W�F�N�V�����s��̕ύX�Ɠo�^������
void CameraComponent::ChangeProjectionMat(float _fov, float _near, float _far)
{
	m_fov = _fov;
	m_near = _near;
	m_far = _far;

	// �v���W�F�N�V�����s��̍č쐬
	Matrix4 proj = 
		Matrix4::CreatePerspectiveFOV(Math::ToRadians(m_fov),
		                              static_cast<float>(GAME_CONFIG->GetScreenWidth()),
		                              static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		                              m_near, m_far);

	// �v���W�F�N�V�����s��Z�b�g
	SetProjectionMatrix(proj);
}

/// <summary>
/// �J�����̃A�N�e�B�u��
/// ���A�N�e�B�u�ȃJ�����̏�񂪃����_�����O�ɓK�p�����
/// </summary>
void CameraComponent::SetActive()
{
	// �J�����X�e�[�^�X���A�N�e�B�u��
	m_cameraStatus = CAMERA_STATUS::ACTIVE;

	// �v���W�F�N�V�����s��Z�b�g
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(m_fov),
		static_cast<float>(GAME_CONFIG->GetScreenWidth()),
		static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		m_near, m_far);
	RENDERER->SetProjectionMatrix(projection);

	GAME_INSTANCE.SetCamera(this);
}

/// <summary>
/// �J�����̔�A�N�e�B�u��
/// </summary>
void CameraComponent::SetDeactive()
{
	m_cameraStatus = CAMERA_STATUS::DEACTIVE;
}

const Vector3& CameraComponent::GetTargetPos() const
{
	return m_owner->GetPosition();
}

/// <summary>
/// �J����FOV�̃Z�b�^�[
/// �ύX���Ƀv���W�F�N�V�����s��̍X�V���s��
/// </summary>
/// <param name="_fov"> �C�ӂ�FOV�l </param>
void CameraComponent::SetFOV(float _fov)
{
	m_fov = _fov;
	ChangeProjectionMat(m_fov, m_near, m_far);
}

/// <summary>
/// �J�����j�A�\�̃Z�b�^�[
/// �ύX���Ƀv���W�F�N�V�����s��̍X�V���s��
/// </summary>
/// <param name="_near"> �C�ӂ̃j�A�\�l </param>
void CameraComponent::SetNear(float _near)
{
	m_near = _near;
	ChangeProjectionMat(m_fov, m_near, m_far);

}

/// <summary>
/// �J�����t�@�[�̃Z�b�^�[
/// </summary>
/// <param name="_far"> �C�ӂ̃t�@�[�l </param>
void CameraComponent::SetFar(float _far)
{
	m_far = _far;
	ChangeProjectionMat(m_fov, m_near, m_far);
}

/// <summary>
/// �`��ɗp����v���W�F�N�V�����s��̍X�V
/// </summary>
/// <param name="_proj"> �V�F�[�_�[�ɓ]������v���W�F�N�V�����s�� </param>
void CameraComponent::SetProjectionMatrix(const Matrix4& _proj)
{
	RENDERER->SetProjectionMatrix(_proj);
}

/// <summary>
/// �`��ɗp����r���[�s��̍X�V
/// </summary>
/// <param name="_view"> �V�F�[�_�[�ɓ]������r���[�s�� </param>
void CameraComponent::SetViewMatrix(const Matrix4 & _view)
{
	RENDERER->SetViewMatrix(_view);
}
