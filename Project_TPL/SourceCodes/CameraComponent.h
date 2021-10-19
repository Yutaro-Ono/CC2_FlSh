#pragma once
#include "Component.h"
#include "Math.h"
#include "GameMain.h"

class CameraComponent : public Component
{
public:

	// カメラのステータス
	enum class CAMERA_STATUS
	{
		DEACTIVE = 0,
		ACTIVE
	};

	CameraComponent(class Actor* _target, int _updateOrder = 200);
	~CameraComponent();

	void ChangeProjectionMat(float _fov, float _near, float _far);

	void SetActive();
	void SetDeactive();

	const Vector3& GetPosition() const { return m_position; }
	const Vector3& GetTargetPos() const;

	const Matrix4& GetViewMatrix() const { return GAME_INSTANCE.GetViewMatrix(); }

	void SetFOV(float _fov);
	void SetNear(float _near);
	void SetFar(float _far);


protected:

	CAMERA_STATUS m_cameraStatus;

	void SetProjectionMatrix(const Matrix4& _proj);
	void SetViewMatrix(const Matrix4& _view);

	Vector3 m_position;       // カメラの座標

	float m_fov;
	float m_near;
	float m_far;
};