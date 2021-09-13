#include "TPSCamera.h"
#include "Input.h"
#include "InputController.h"
#include "Actor.h"

const float maxLookDownAngle = Math::ToRadians(45.0f); // 最大仰角
const float minLookDownAngle = Math::ToRadians(-10.0f); // 最小仰角

TPSCamera::TPSCamera(Actor* _target)
    :CameraComponent(_target)
    ,m_cameraLength(Vector3::Zero)
    ,m_rotateZAngle(0.0f)
    ,m_lookDownAngle(minLookDownAngle)
    ,m_viewTarget(_target->GetPosition())
    ,m_adjustTargetPos(Vector2(0.0f, -50.0f))
{
    m_position = Vector3(100.0f, 100.0f, 100.0f);

}

TPSCamera::~TPSCamera()
{
}

void TPSCamera::Update(float _deltaTime)
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

    // キー入力
    const float rotate = 0.5f * _deltaTime;
    if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
    {
        m_rotateZAngle += rotate;
    }
    if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
    {
        m_rotateZAngle -= rotate;
    }
    if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_UP))
    {
        m_lookDownAngle += rotate;
    }
    if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_DOWN))
    {
        m_lookDownAngle -= rotate;
    }

    // パッド入力
    Vector2 axisR;
    axisR = CONTROLLER_INSTANCE.GetRAxisVec();
    m_rotateZAngle += axisR.x * rotate;
    m_lookDownAngle += axisR.y * rotate;

    // 見降ろし角度の角度制限
    if (m_lookDownAngle < minLookDownAngle)
    {
        m_lookDownAngle = minLookDownAngle;
    }
    if (m_lookDownAngle > maxLookDownAngle)
    {
        m_lookDownAngle = maxLookDownAngle;
    }

    // ヨー回転・ピッチ回転
    Vector3 rotatePos;
    rotatePos.x = m_cameraLength.x * cosf(m_lookDownAngle) * cosf(m_rotateZAngle);
    rotatePos.y = m_cameraLength.y * cosf(m_lookDownAngle) * sinf(m_rotateZAngle);
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