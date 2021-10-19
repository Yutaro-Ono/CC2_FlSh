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
	// ゲームからこのカメラを削除
	GAME_INSTANCE.InActiveCamera(this);
}

// プロジェクション行列の変更と登録し直し
void CameraComponent::ChangeProjectionMat(float _fov, float _near, float _far)
{
	m_fov = _fov;
	m_near = _near;
	m_far = _far;

	// プロジェクション行列の再作成
	Matrix4 proj = 
		Matrix4::CreatePerspectiveFOV(Math::ToRadians(m_fov),
		                              static_cast<float>(GAME_CONFIG->GetScreenWidth()),
		                              static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		                              m_near, m_far);

	// プロジェクション行列セット
	SetProjectionMatrix(proj);
}

/// <summary>
/// カメラのアクティブ化
/// ※アクティブなカメラの情報がレンダリングに適用される
/// </summary>
void CameraComponent::SetActive()
{
	// カメラステータスをアクティブに
	m_cameraStatus = CAMERA_STATUS::ACTIVE;

	// プロジェクション行列セット
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(m_fov),
		static_cast<float>(GAME_CONFIG->GetScreenWidth()),
		static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		m_near, m_far);
	RENDERER->SetProjectionMatrix(projection);

	GAME_INSTANCE.SetCamera(this);
}

/// <summary>
/// カメラの非アクティブ化
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
/// カメラFOVのセッター
/// 変更時にプロジェクション行列の更新も行う
/// </summary>
/// <param name="_fov"> 任意のFOV値 </param>
void CameraComponent::SetFOV(float _fov)
{
	m_fov = _fov;
	ChangeProjectionMat(m_fov, m_near, m_far);
}

/// <summary>
/// カメラニア―のセッター
/// 変更時にプロジェクション行列の更新も行う
/// </summary>
/// <param name="_near"> 任意のニア―値 </param>
void CameraComponent::SetNear(float _near)
{
	m_near = _near;
	ChangeProjectionMat(m_fov, m_near, m_far);

}

/// <summary>
/// カメラファーのセッター
/// </summary>
/// <param name="_far"> 任意のファー値 </param>
void CameraComponent::SetFar(float _far)
{
	m_far = _far;
	ChangeProjectionMat(m_fov, m_near, m_far);
}

/// <summary>
/// 描画に用いるプロジェクション行列の更新
/// </summary>
/// <param name="_proj"> シェーダーに転送するプロジェクション行列 </param>
void CameraComponent::SetProjectionMatrix(const Matrix4& _proj)
{
	RENDERER->SetProjectionMatrix(_proj);
}

/// <summary>
/// 描画に用いるビュー行列の更新
/// </summary>
/// <param name="_view"> シェーダーに転送するビュー行列 </param>
void CameraComponent::SetViewMatrix(const Matrix4 & _view)
{
	RENDERER->SetViewMatrix(_view);
}
