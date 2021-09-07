#pragma once
#include "CameraComponent.h"


class ThirdPersonCarCamera : public CameraComponent
{
public:

	ThirdPersonCarCamera(class PlayerCar* in_owner);
	~ThirdPersonCarCamera();

	void Update(float in_deltaTime) override;           // �X�V����

	void ProcessInput(float in_deltaTime) override;     // ���͏���

	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	float GetPitch() const { return m_pitch; }          // �s�b�`�̃Q�b�^�[
	float GetYaw() const { return m_yaw; }              // ���[�̃Q�b�^�[
	void SetDistance(float in_dist);                    // �J���������̃Z�b�^�[
	// �J�����Ǐ]�������s�����ǂ����̃Z�b�^�[
	void SetChaseOwnerForward(bool in_chase) { m_chaseOwnerForward = in_chase; }
	// �J�����O�i�x�N�g���̃Q�b�^�[�E�Z�b�^�[
	const Vector3& GetForward() { return m_forwardVec; }
	void SetCameraForward(const Vector3& in_forward) { m_forwardVec = in_forward; }

private:

	// �I�[�i�[�̌�������苗���ɃJ�����ʒu�𒲐����A���̍��W��Ԃ�
	const Vector3& ComputeCameraPos() const;

	Vector3 m_offset;                              // �O�l�̎��_���^�[�Q�b�g����̋����I�t�Z�b�g

	Vector3 m_forwardVec;                          // �J�����̑O�i�x�N�g��
	Vector3 m_upVec;                               // �J�����̏���x�N�g��

	Vector3 m_velocity;                            // �J���������x

	Vector3 m_ownerForward;                        // �I�[�i�[�A�N�^�̈ړ����Ɏg�p����O�i�x�N�g��(�J�����)

	float m_pitch;                                 // �s�b�`�֘A

	float m_yaw;                                   // ���[���x(/�b)

	float m_distance;                              // �^�[�Q�b�g�܂ł̋���

	// �J�����̑O�i�x�N�g�����I�[�i�[�A�N�^�̑O�i�x�N�g���֒Ǐ]�����邩 (�f�t�H���g�FOFF)
	bool m_chaseOwnerForward;

	class PlayerCar* m_playerCar;                  // �^�[�Q�b�g�A�N�^(��蕨)

	// �}�E�X����
	Vector2 m_mousePos;                            // �}�E�X�̍��W
	Vector2 m_frameMousePos;                       // �}�E�X�̑O�t���[���̍��W

	static const float CAMERA_SENSITIVITY;
	static const Vector3 DEFAULT_DISTANCE_OFFSET;
	static const float MIN_TARGET_DISTANCE;        // �^�[�Q�b�g�܂ł̍ŒZ����
	static const float MAX_TARGET_DISTANCE;        // �^�[�Q�b�g�܂ł̍ő勗��
};