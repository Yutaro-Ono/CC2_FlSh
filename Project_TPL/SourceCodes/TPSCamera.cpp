#include "TPSCamera.h"
#include "GameMain.h"
#include "Input.h"
#include "Mouse.h"
#include "InputController.h"
#include "Mouse.h"
#include "Actor.h"

const float maxLookDownAngle = Math::ToRadians(45.0f);  // 最大仰角
const float minLookDownAngle = Math::ToRadians(-10.0f); // 最小仰角

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
    // カメラがアクティブ状態時のみ更新を行う
    if (m_cameraStatus == CAMERA_STATUS::ACTIVE)
    {
        // ばね定数
        const float springConstant = 128.0f;

        // 接近係数
        // ばね定数から減衰を計算
        const float dumper = 9.0f * Math::Sqrt(springConstant);
        Vector3 diff;
        Vector3 playerPos = m_owner->GetPosition();

        // 差分ベクトルからプレーヤー位置まで近づける
        diff = playerPos - m_viewTarget;
        diff = dumper * _deltaTime * diff;

        // カメラ回転値
        const float rotate = 0.5f * _deltaTime;

        // パッド入力
        if (m_isActivePad)
        {
            UpdateGamePad(rotate, _deltaTime);
        }
        // キーボード入力
        else
        {
            UpdateKeyBoard(rotate, _deltaTime);
        }

        // ヨー回転・ピッチ回転
        Vector3 rotatePos;
        rotatePos.x = m_cameraLength.x * cosf(m_lookDownAngle) * cosf(m_rotateAngleZ);
        rotatePos.y = m_cameraLength.y * cosf(m_lookDownAngle) * sinf(m_rotateAngleZ);
        rotatePos.z = m_cameraLength.z * sinf(m_lookDownAngle);

        // 注視点・カメラ位置をセット
        m_position = rotatePos + playerPos;
        m_viewTarget = playerPos;

        // ビュー行列を更新
        Matrix4 view = Matrix4::CreateLookAt(m_position, m_viewTarget, Vector3(0.0f, 0.0f, 1.0f));
        // 調整ベクトル
        Vector3 dist = Vector3(m_adjustTargetPos.x, m_adjustTargetPos.y, 1.0f);
        view = view * Matrix4::CreateTranslation(dist);
        SetViewMatrix(view);
    }

}

void TPSCamera::UpdateMouse(const float _rotate, float _deltaTime)
{
    // マウスのスクリーン座標を取得
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

    // 見降ろし角度の角度制限
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

    // 見降ろし角度の角度制限
    if (m_lookDownAngle < minLookDownAngle)
    {
        m_lookDownAngle = minLookDownAngle;
    }
    if (m_lookDownAngle > maxLookDownAngle)
    {
        m_lookDownAngle = maxLookDownAngle;
    }
}

