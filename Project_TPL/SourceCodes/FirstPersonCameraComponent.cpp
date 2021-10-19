#include "FirstPersonCameraComponent.h"
#include "GameMain.h"
#include "Input.h"
#include "Mouse.h"
#include "InputController.h"
#include "Mouse.h"
#include "Actor.h"

const float FirstPersonCameraComponent::MAX_PITCH_ANGLE = Math::ToRadians(85.0f);     // �ő�p
const float FirstPersonCameraComponent::MIN_PITCH_ANGLE = Math::ToRadians(-85.0f);    // �ŏ��p
const float FirstPersonCameraComponent::CAMERA_SENSITIVITY = 0.5f;                    // �J�����̔������x

FirstPersonCameraComponent::FirstPersonCameraComponent(Actor* _owner)
	:CameraComponent(_owner)
    ,m_yaw(0.0f)
    ,m_pitch(0.0f)
    ,m_roll(0.0f)
    ,m_cameraForwardVec(Vector3(1.0f, 0.0f, 0.0f))
    ,m_viewTarget(_owner->GetPosition())
    ,m_cameraSensitivity(CAMERA_SENSITIVITY)
{
    m_fov = 80.0f;
    m_position = m_owner->GetPosition();
    m_isActivePad = CONTROLLER_INSTANCE.IsAvailable();
}

FirstPersonCameraComponent::~FirstPersonCameraComponent()
{
}

void FirstPersonCameraComponent::Update(float _deltaTime)
{
    // �J�������A�N�e�B�u��Ԏ��̂ݍX�V���s��
    if (m_cameraStatus == CAMERA_STATUS::ACTIVE)
    {
        // �O���x�N�g���̍X�V
        m_cameraForwardVec = m_viewTarget - m_position;
        //m_cameraForwardVec.z = 0.0f;
        m_cameraForwardVec.Normalize();

        // ���[��]�E�s�b�`��]����S�̂̉�]�p�x���Z�o
        // (��)Z�A�b�v�̂���y����z�������ւ�
        Vector3 rotatePos;
        rotatePos.x = cosf(m_pitch) * cosf(m_yaw);
        rotatePos.y = cosf(m_pitch) * sinf(m_yaw);
        rotatePos.z = sinf(m_pitch);

        // �^�[�Q�b�g�ƂȂ�v���C���[�̍��W���擾
        Vector3 playerPos = m_owner->GetPosition() + Vector3(0.0f, 0.0f, 120.0f);

        m_viewTarget = playerPos;
        m_position = rotatePos + playerPos;

        // �v���W�F�N�V�����s����X�V
        Matrix4 proj = Matrix4::CreatePerspectiveFOV(m_fov, 
            static_cast<float>(GAME_CONFIG->GetScreenWidth()), 
            static_cast<float>(GAME_CONFIG->GetScreenHeight()), m_near, m_far);
        SetProjectionMatrix(proj);
        // �r���[�s����X�V
        Matrix4 view = Matrix4::CreateLookAt(m_position, m_viewTarget, Vector3(0.0f, 0.0f, -1.0f));
        SetViewMatrix(view);
    }

}

/// <summary>
/// ���͏����̍X�V(�e�N���X����I�[�o�[���C�h)
/// </summary>
/// <param name="_deltaTime"> �f���^�^�C�� </param>
void FirstPersonCameraComponent::ProcessInput(float _deltaTime)
{
    // �J������]�l
    const float rotate = m_cameraSensitivity * _deltaTime;

    // �p�b�h�ڑ���
    // �p�b�h�̉E�X�e�B�b�N�ɂ���]�̍X�V
    if (m_isActivePad)
    {
        UpdateGamePad(rotate, _deltaTime);
    }
    // �p�b�h��ڑ���
    // �}�E�X���͂ɂ���]�̍X�V
    else
    {
        UpdateMouse(rotate, _deltaTime);
    }

    // ���~�낵�p�x�̊p�x��������
    if (m_pitch < MIN_PITCH_ANGLE)
    {
        m_pitch = MIN_PITCH_ANGLE;
    }
    if (m_pitch > MAX_PITCH_ANGLE)
    {
        m_pitch = MAX_PITCH_ANGLE;
    }
}


void FirstPersonCameraComponent::UpdateMouse(const float _rotate, float _deltaTime)
{
    // �}�E�X���͂����΃��[�h���̂ݍX�V
    if (MOUSE_INSTANCE.GetIsRelative())
    {
        m_yaw += MOUSE_INSTANCE.GetPositionX() * _rotate * 0.1f;
        m_pitch += MOUSE_INSTANCE.GetPositionY() * _rotate * 0.1f;
    }
}

void FirstPersonCameraComponent::UpdateKeyBoard(const float _rotate, float _deltaTime)
{
    if (MOUSE_INSTANCE.GetIsRelative())
    {
        m_yaw += MOUSE_INSTANCE.GetPositionX() * _rotate;
        m_pitch += MOUSE_INSTANCE.GetPositionY() * _rotate;
    }
}

void FirstPersonCameraComponent::UpdateGamePad(const float _rotate, float _deltaTime)
{
    Vector2 axisR;
    axisR = CONTROLLER_INSTANCE.GetRAxisVec();
    m_yaw += axisR.x * _rotate;
    m_pitch += axisR.y * _rotate;
}
