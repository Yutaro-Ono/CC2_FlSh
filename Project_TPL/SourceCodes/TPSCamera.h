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
	Vector3 m_cameraLength;       // �J��������
	float m_rotateZAngle;         // �J�������͉�]������]�p
	float m_lookDownAngle;        // �J�������~�낵������]�p
	Vector3 m_cameraForwardVec;   // �O������
	Vector3 m_viewTarget;
	Vector2 m_adjustTargetPos;    // �^�[�Q�b�g���W�̔������p(x = �����Ay = ����)
};