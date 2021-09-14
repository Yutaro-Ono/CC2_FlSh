#pragma once
#include "CameraComponent.h"


class TPSCamera : public CameraComponent
{
public:
	TPSCamera(Actor* _target);
	~TPSCamera();
	void Update(float _deltaTime) override;

	void UpdateMouse(const float _rotate, float _deltaTime);
	void UpdateKeyBoard(const float _rotate, float _deltaTime);
	void UpdateGamePad(const float _rotate, float _deltaTime);

	void SetRotateZAngle(float _angle) { m_rotateZAngle = _angle; }
	void SetLookDownAngle(float _angle) { m_lookDownAngle = _angle; }
	void SetCameraLength(const Vector3& _length) { m_cameraLength = _length; }
	void SetAdjustTargetPos(const Vector2& _adjust) { m_adjustTargetPos = _adjust; }
	Vector3& GetCameraForwardVec() { return m_cameraForwardVec; }

private:

	bool m_isActivePad;           // �p�b�h���L�����ǂ���
	Vector3 m_cameraLength;       // �J��������
	float m_rotateZAngle;         // �J�������͉�]������]�p
	float m_lookDownAngle;        // �J�������~�낵������]�p
	Vector3 m_cameraForwardVec;   // �O������
	Vector3 m_viewTarget;         // �J�������^�[�Q�b�g������W
	Vector2 m_adjustTargetPos;    // �^�[�Q�b�g���W�̔������p(x = �����Ay = ����)
};