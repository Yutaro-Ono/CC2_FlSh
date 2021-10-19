#include "FirstPersonCameraComponent.h"
#include "GameMain.h"
#include "Input.h"
#include "Mouse.h"
#include "InputController.h"
#include "Mouse.h"
#include "Actor.h"

const float FirstPersonCameraComponent::MAX_PITCH_ANGLE = Math::ToRadians(85.0f);     // 最大仰角
const float FirstPersonCameraComponent::MIN_PITCH_ANGLE = Math::ToRadians(-85.0f);    // 最小仰角
const float FirstPersonCameraComponent::CAMERA_SENSITIVITY = 0.5f;                    // カメラの反応速度

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
    // カメラがアクティブ状態時のみ更新を行う
    if (m_cameraStatus == CAMERA_STATUS::ACTIVE)
    {
        // 前方ベクトルの更新
        m_cameraForwardVec = m_viewTarget - m_position;
        //m_cameraForwardVec.z = 0.0f;
        m_cameraForwardVec.Normalize();

        // ヨー回転・ピッチ回転から全体の回転角度を算出
        // (注)Zアップのためy軸とz軸を入れ替え
        Vector3 rotatePos;
        rotatePos.x = cosf(m_pitch) * cosf(m_yaw);
        rotatePos.y = cosf(m_pitch) * sinf(m_yaw);
        rotatePos.z = sinf(m_pitch);

        // ターゲットとなるプレイヤーの座標を取得
        Vector3 playerPos = m_owner->GetPosition() + Vector3(0.0f, 0.0f, 120.0f);

        m_viewTarget = playerPos;
        m_position = rotatePos + playerPos;

        // プロジェクション行列を更新
        Matrix4 proj = Matrix4::CreatePerspectiveFOV(m_fov, 
            static_cast<float>(GAME_CONFIG->GetScreenWidth()), 
            static_cast<float>(GAME_CONFIG->GetScreenHeight()), m_near, m_far);
        SetProjectionMatrix(proj);
        // ビュー行列を更新
        Matrix4 view = Matrix4::CreateLookAt(m_position, m_viewTarget, Vector3(0.0f, 0.0f, -1.0f));
        SetViewMatrix(view);
    }

}

/// <summary>
/// 入力処理の更新(親クラスからオーバーライド)
/// </summary>
/// <param name="_deltaTime"> デルタタイム </param>
void FirstPersonCameraComponent::ProcessInput(float _deltaTime)
{
    // カメラ回転値
    const float rotate = m_cameraSensitivity * _deltaTime;

    // パッド接続時
    // パッドの右スティックによる回転の更新
    if (m_isActivePad)
    {
        UpdateGamePad(rotate, _deltaTime);
    }
    // パッド非接続時
    // マウス入力による回転の更新
    else
    {
        UpdateMouse(rotate, _deltaTime);
    }

    // 見降ろし角度の角度制限処理
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
    // マウス入力が相対モード時のみ更新
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
