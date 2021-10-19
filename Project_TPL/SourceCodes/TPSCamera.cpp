#include "TPSCamera.h"
#include "GameMain.h"
#include "Input.h"
#include "Mouse.h"
#include "InputController.h"
#include "Mouse.h"
#include "Actor.h"

const float maxLookDownAngle = Math::ToRadians(45.0f);  // �ő�p
const float minLookDownAngle = Math::ToRadians(-10.0f); // �ŏ��p

TPSCamera::TPSCamera(Actor* _target)
    :CameraComponent(_target)
    ,m_cameraLength(Vector3::Zero)
    ,m_rotateAngleZ(0.0f)
    ,m_lookDownAngle(minLookDownAngle)
    ,m_viewTarget(_target->GetPosition())
    ,m_adjustTargetPos(Vector2(0.0f, -50.0f))
{
    m_position = Vector3(100.0f, 100.0f, 100.0f);
    m_isActivePad = CONTROLLER_INSTANCE.IsAvailable();
}

TPSCamera::~TPSCamera()
{
}

void TPSCamera::Update(float _deltaTime)
{
    // �J�������A�N�e�B�u��Ԏ��̂ݍX�V���s��
    if (m_cameraStatus == CAMERA_STATUS::ACTIVE)
    {
        // �΂˒萔
        const float springConstant = 128.0f;

        // �ڋߌW��
        // �΂˒萔���猸�����v�Z
        const float dumper = 9.0f * Math::Sqrt(springConstant);
        Vector3 diff;
        Vector3 playerPos = m_owner->GetPosition();

        // �����x�N�g������v���[���[�ʒu�܂ŋ߂Â���
        diff = playerPos - m_viewTarget;
        diff = dumper * _deltaTime * diff;

        // �J������]�l
        const float rotate = 0.5f * _deltaTime;

        // �p�b�h����
        if (m_isActivePad)
        {
            UpdateGamePad(rotate, _deltaTime);
        }
        // �L�[�{�[�h����
        else
        {
            UpdateKeyBoard(rotate, _deltaTime);
        }

        // ���[��]�E�s�b�`��]
        Vector3 rotatePos;
        rotatePos.x = m_cameraLength.x * cosf(m_lookDownAngle) * cosf(m_rotateAngleZ);
        rotatePos.y = m_cameraLength.y * cosf(m_lookDownAngle) * sinf(m_rotateAngleZ);
        rotatePos.z = m_cameraLength.z * sinf(m_lookDownAngle);

        // �����_�E�J�����ʒu���Z�b�g
        m_position = rotatePos + playerPos;
        m_viewTarget = playerPos;

        // �r���[�s����X�V
        Matrix4 view = Matrix4::CreateLookAt(m_position, m_viewTarget, Vector3(0.0f, 0.0f, 1.0f));
        // �����x�N�g��
        Vector3 dist = Vector3(m_adjustTargetPos.x, m_adjustTargetPos.y, 1.0f);
        view = view * Matrix4::CreateTranslation(dist);
        SetViewMatrix(view);
    }

}

void TPSCamera::UpdateMouse(const float _rotate, float _deltaTime)
{
    // �}�E�X�̃X�N���[�����W���擾
    Vector2 mousePos = MOUSE_INSTANCE.GetPosition();

    //mousePos.x

}

void TPSCamera::UpdateKeyBoard(const float _rotate, float _deltaTime)
{
    //if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
    //{
    //    m_rotateAngleZ += _rotate;
    //}
    //if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
    //{
    //    m_rotateAngleZ -= _rotate;
    //}
    //if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_UP))
    //{
    //    m_lookDownAngle += _rotate;
    //}
    //if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_DOWN))
    //{
    //    m_lookDownAngle -= _rotate;
    //}


    if (MOUSE_INSTANCE.GetIsRelative())
    {
        m_rotateAngleZ += MOUSE_INSTANCE.GetPositionX() * _rotate * 0.1f;
        m_lookDownAngle += MOUSE_INSTANCE.GetPositionY() * _rotate * 0.1f;
    }

    // ���~�낵�p�x�̊p�x����
    if (m_lookDownAngle < minLookDownAngle)
    {
        m_lookDownAngle = minLookDownAngle;
    }
    if (m_lookDownAngle > maxLookDownAngle)
    {
        m_lookDownAngle = maxLookDownAngle;
    }

    //printf("Mouse Position X : %f\n", m_rotateAngleZ);
    //printf("Mouse Position Y : %f\n", m_lookDownAngle);
}

void TPSCamera::UpdateGamePad(const float _rotate, float _deltaTime)
{
    Vector2 axisR;
    axisR = CONTROLLER_INSTANCE.GetRAxisVec();
    m_rotateAngleZ += axisR.x * _rotate;
    m_lookDownAngle += axisR.y * _rotate;

    // ���~�낵�p�x�̊p�x����
    if (m_lookDownAngle < minLookDownAngle)
    {
        m_lookDownAngle = minLookDownAngle;
    }
    if (m_lookDownAngle > maxLookDownAngle)
    {
        m_lookDownAngle = maxLookDownAngle;
    }
}

