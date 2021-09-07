//----------------------------------------------------------------------------------+
// @file        Vehicle.h
// @brief       ��蕨�x�[�X�N���X�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/ 7   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Tag.h"
#include "Math.h"


class Vehicle
{

public:

	// ��ʓI�ȏ�蕨�̏��
	enum class VEHICLE_STATE
	{
		STATE_ACTIVE = 0,
		STATE_PAUSED,
		STATE_DESTROY
	};

	Vehicle(OBJECT_TAG _tag);
	virtual ~Vehicle();

	void Update(float _deltaTime);                          // �X�V��������
	void UpdateComponents(float _deltaTime);                // ���̃N���X��e�Ƃ���R���|�[�l���g�̍X�V����
	virtual void UpdateVehicle(float _deltaTime) = 0;       // �p����ŗL�̍X�V����(�������z�֐�)
	void ProcessInput(float _deltaTime);                    // �R���|�[�l���g�֘A�̓��͏��X�V

	void AddComponent(class Component* _comp);              // �R���|�[�l���g�̒ǉ�����
	void RemoveComponent(class Component* _comp);           // �R���|�[�l���g�̍폜����

	void RotateToNewForward(const Vector3& _forward);       // �O�i�x�N�g���̍X�V

	//-----------------------------------------------------------------------------------+
	// Getter / Setter
	//-----------------------------------------------------------------------------------+
	// �|�W�V����
	const Vector3& GetPosition() const { return m_position; }
	void SetPosition(const Vector3& _pos) { m_position = _pos; m_recomputeWorldTransform = true; }
	// �X�P�[��
	const Vector3& GetScale() const { return m_scale; }
	void SetScale(float _scale) { m_scale = Vector3(_scale, _scale, _scale); m_recomputeWorldTransform = true; }
	void SetScale(const Vector3& _scalev3) { m_scale = _scalev3; m_recomputeWorldTransform = true; }
	// ���[�e�[�V����
	const Quaternion& GetRotation() const { return m_rotation; }
	void SetRotation(const Quaternion& _rotate) { m_rotation = _rotate; m_recomputeWorldTransform = true; }

	// ���[���h�ϊ��s��(�����ӁF�Z�b�g�֐����g�p�����ꍇ�A�K�؂ȕϊ��v�Z���s���Ȃ��Ȃ�܂�)
	void SetWorldTransform(const Matrix4& _worldMat) { m_worldTransform = _worldMat; }
	const Matrix4& GetWorldTransform() const { return m_worldTransform; }
	// �x�N�g��
	const Vector3& GetForward() const { return Vector3::Transform(Vector3::UnitX, m_rotation); }     // �O�i
	const Vector3& GetRight() const { return Vector3::Transform(Vector3::UnitY, m_rotation); }       // �E����
	// �X�e�[�g
	VEHICLE_STATE GetState() const { return m_state; }
	void SetState(VEHICLE_STATE _state) { m_state = _state; }


private:

	void ComputeWorldTransform();                    // ���[���h�ϊ��s��̍Čv�Z����



	VEHICLE_STATE   m_state;                         // ��蕨�̏��
	OBJECT_TAG      m_tag;                           // ���̃N���X�̃^�O

	// ���W�n
	Vector3         m_scale;                         // �g�嗦
	Quaternion      m_rotation;                      // ��]
	Vector3         m_position;                      // ���[���h���W
	Matrix4         m_worldTransform;                // ���[���h�ϊ��s��
	bool            m_recomputeWorldTransform;       // ���[���h�ϊ��s��̍Čv�Z�t���O(���׌y����)

	bool            m_accel;                         // �A�N�Z���y�_���𓥂�ł��邩
	bool            m_brake;                         // �u���[�L�y�_���𓥂�ł��邩
	float           m_speed;                         // ��蕨�̃X�s�[�h
	float           m_velocity;                      // �����x
	float           m_weight;                        // ��蕨�̏d��
	float           m_friction;                      // ���C�W��

	bool            m_isRide;                        // �v���C���[�ɏ���Ă��邩�ǂ���


};