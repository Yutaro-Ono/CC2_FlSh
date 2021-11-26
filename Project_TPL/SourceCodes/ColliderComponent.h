//----------------------------------------------------------------------------------+
// @file        ColliderComponent.h
// @brief       �����蔻��R���|�[�l���g�N���X
// @note        Component�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/17   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

// �����蔻��^�C�v�̗�
enum class COLLIDER_TYPE
{
	TYPE_BOX,
	TYPE_WALL,
	TYPE_SLOPE,
	TYPE_SPHERE,
	TYPE_LINE
};

class ColliderComponent : public Component
{

public:

	ColliderComponent(class Actor* _owner, COLLIDER_TYPE _type, int _updateOrder = 100);
	virtual ~ColliderComponent() {};

	OBJECT_TAG GetOwnerTag() { return m_colliderTag; }
	
	CollisionInfo& GetCollisionInfo() { return m_collisionInfo; }
	void SetCollisionInfo(const CollisionInfo& _colInfo) { m_collisionInfo = _colInfo; }
	void ClearCollisionInfo();

	COLLIDER_TYPE GetColliderType() { return m_colliderType; }
	void ChangeColliderType(COLLIDER_TYPE _colType) { m_colliderType = _colType; }

	virtual void OnUpdateWorldTransform() {};

	// Double-Dispatch �p�^�[�����g�p���ďՓ˔���g�ݍ��킹���s��
	virtual bool CollisionDetection(ColliderComponent* _other) = 0;

	// Double-Dispatch �p�^�[���FCollider���m�̂ӂ�܂����`
	virtual bool Check(class BoxColliderComponent* _other);
	virtual bool Check(class WallColliderComponent* _other);
	virtual bool Check(class LineColliderComponent* _other);


protected:

	OBJECT_TAG m_colliderTag;              // �����蔻��p�^�O(�I�[�i�[���Q��)
	COLLIDER_TYPE m_colliderType;          // �����蔻��̎��
	CollisionInfo m_collisionInfo;         // �����蔻����

	friend class PhysicsWorld;
};