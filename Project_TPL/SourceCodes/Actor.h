//-----------------------------------------------------------------------+
// �A�N�^�[�N���X
// �@�\�F�A�N�^�[(��{�I�Ƀ��[���h��Ԃɍ��W��������)�̃x�[�X�N���X
// �����F�x�[�X�N���X
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>
#include "Tag.h"
#include "ActorDebugObject.h"

class Actor
{

public:


	// �A�N�^�[�̃X�e�[�g(���)
	enum ACTOR_STATE
	{
		STATE_ACTIVE,            // �A�N�e�B�u�ł���
		STATE_PAUSED,            // ��~���ł���
		STATE_DEAD,              // �폜������s��
		STATE_ALLSTATE           // �S�X�e�[�g
	};

	Actor(OBJECT_TAG _tag);
	virtual ~Actor();     // ���z�f�X�g���N�^

	void Update(float _deltaTime);                  // �X�V����(����)
	void UpdateComponents(float _deltaTime);        // �X�V���� (�R���|�[�l���g)
	virtual void UpdateActor(float _deltaTime);     // �X�V���� (�A�N�^�[�ŗL����:�I�[�o�[���C�h�\)
	void ProcessInput(float _deltaTime);            // ���͏���

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
	void SetRotation(const Quaternion& _rotation) { m_rotation = _rotation; m_recomputeWorldTransform = true; }

	// ���[���h�ϊ��s��
	void ComputeWorldTransform();
	void SetWorldTransform(const Matrix4& _mat) { m_worldTransform = _mat; }
	const Matrix4& GetWorldTransform() const { return m_worldTransform; }
	// �x�N�g��
	const Vector3& GetForward() const { return Vector3::Transform(Vector3::UnitX, m_rotation); }     // �O�i
	void RotateToNewForward(const Vector3& _forward);                                              // �O�i�x�N�g���̃Z�b�g
	const Vector3& GetRight() const { return Vector3::Transform(Vector3::UnitY, m_rotation); }       // �E����
	// �X�s�[�h
	float GetSpeed() const { return m_speed; }
	void SetSpeed(float _speed) { m_speed = _speed; }
	// �X�e�[�g
	Actor::ACTOR_STATE GetState() const { return m_state; }
	void SetState(Actor::ACTOR_STATE _state) { m_state = _state; }
	// �Ǘ�ID
	int GetID() { return m_ID; }
	// �R���|�[�l���g
	void AddComponent(class Component* _comp);
	void RemoveComponent(class Component* _comp);
	// ���b�V���R���|�[�l���g
	class MeshComponent* GetMeshComponent() { return m_meshComp; }

	// �̗�
	void Sethealth(unsigned int _health) { m_health = _health; }
	const unsigned int& GetHealth() const { return m_health; }
	void RecoverHealth(unsigned int _recoverVal);
	void ReduceHealth(unsigned int _damage);


protected:


	Actor::ACTOR_STATE m_state;                           // �X�e�[�g(���)
	OBJECT_TAG m_tag;                               // �x�[�X�N���X���ʃ^�O

	Vector3 m_scale;                                // �X�P�[��(�g��E�k����)
	Quaternion m_rotation;                          // ��]�p
	Vector3 m_position;                             // �|�W�V����
	Matrix4 m_worldTransform;                       // ���[���h�ϊ��s��

	float m_radian;

	unsigned int m_health;                          // �̗�

	float m_speed;                                  // �ړ����x

	bool m_recomputeWorldTransform;                 // ���[���h���W�ϊ����s����

	class MeshComponent* m_meshComp;                // ���b�V���R���|�[�l���g�̃|�C���^

	std::vector<class Component*> m_components;     // �R���|�[�l���g�z��(�A�N�^�[�̎��R���|�[�l���g���i�[)

	int m_ID;                                       // �A�N�^�[ID
	static int m_globalActorNo;                     // �Q�[���S�̂ł̃A�N�^�[�̔ԍ�


	class ActorDebugObject* m_debugObj;
	// �A�N�^�[�p�f�o�b�O�N���X�̃t�����h�錾
	friend class ActorDebugObject;

};