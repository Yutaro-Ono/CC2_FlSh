#pragma once
#include "CameraComponent.h"


class FirstPersonCameraComponent : public CameraComponent
{
public:
	FirstPersonCameraComponent(class Actor* _owner);
	~FirstPersonCameraComponent();
	void Update(float _deltaTime) override;

	void ProcessInput(float _deltaTime) override;

	void UpdateMouse(const float _rotate, float _deltaTime);
	void UpdateKeyBoard(const float _rotate, float _deltaTime);
	void UpdateGamePad(const float _rotate, float _deltaTime);

	float GetYaw() { return m_yaw; }
	float GetPitch() { return m_pitch; }
	void SetYawAngle(float _angle) { m_yaw = _angle; }
	void SetPitchAngle(float _angle) { m_pitch = _angle; }
	Vector3& GetCameraForwardVec() { return m_cameraForwardVec; }

	void SetHeadBoneNum(unsigned int _headBoneNum) { m_ownerHeadBoneNum = _headBoneNum; }

private:

	bool m_isActivePad;                 // �p�b�h���L�����ǂ���
	float m_yaw;                        // �J�������[(Z����]�l)
	float m_pitch;                      // �J�����s�b�`(Y����]�l)
	float m_roll;                       // �J�������[��(X����]�l)
	Vector3 m_cameraForwardVec;         // �O�������x�N�g��
	Vector3 m_viewTarget;               // �J�������^�[�Q�b�g������W
	float m_cameraSensitivity;          // �J�������͎��̔������x
	unsigned int m_ownerHeadBoneNum;    // �w�b�h�{�[���̔ԍ�(���W���v���C���[�̓��ɕR�t����ۂɎg�p)

	static const float MAX_PITCH_ANGLE;     // �s�b�`�̍ő�p�x
	static const float MIN_PITCH_ANGLE;     // �s�b�`�̍ŏ��p�x
	static const float CAMERA_SENSITIVITY;  // �J�����̔������x

};