#pragma once
#include "CameraComponent.h"


class TPSCamera : public CameraComponent
{
public:
	TPSCamera(Actor* _target);
	~TPSCamera();
	void Update(float _deltaTime) override;

	void SetRotateZAngle(float _angle) { m_rotateZAngle = _angle; }
	void SetLookDownAngle(float _angle) { m_lookDownAngle = _angle; }
	void SetCameraLength(const Vector3& _length) { m_cameraLength = _length; }
	void SetAdjustTargetPos(const Vector2& _adjust) { m_adjustTargetPos = _adjust; }
	Vector3& GetCameraForwardVec() { return m_cameraForwardVec; }

private:
	Vector3 m_cameraLength;       // カメラ距離
	float m_rotateZAngle;         // カメラ周囲回転方向回転角
	float m_lookDownAngle;        // カメラ見降ろし方向回転角
	Vector3 m_cameraForwardVec;   // 前方方向
	Vector3 m_viewTarget;
	Vector2 m_adjustTargetPos;    // ターゲット座標の微調整用(x = 水平、y = 高さ)
};