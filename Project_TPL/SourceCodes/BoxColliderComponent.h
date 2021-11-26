//-----------------------------------------------------------------------+
// BoxCollider�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "ColliderComponent.h"
#include "Collision.h"
#include "PhysicsWorld.h"

class BoxColliderComponent : public ColliderComponent
{
public:


	BoxColliderComponent(class Actor* _owner, int _updateOrder = 100);
	~BoxColliderComponent();

	void OnUpdateWorldTransform() override;                                 // ���[���h�ϊ���
	void SetObjectBox(const AABB& _box) { m_objectBox = _box; }         // �����蔻��p���E�{�b�N�X���Z�b�g
	const AABB& GetWorldBox() const { return m_worldBox; }                  // ���[���h��ԏ�ł̋��E�{�b�N�X���擾
	void SetArrowRotate(bool _value) { m_rotatable = _value; }          // ��]�������邩

	bool CollisionDetection(ColliderComponent* _other) override;

protected:

	bool Check(class BoxColliderComponent* _other) override;
	bool Check(class WallColliderComponent* _other) override;
	bool Check(class LineColliderComponent* _other) override;



	AABB m_objectBox;                                                       // �I�u�W�F�N�g���(�ϊ��O)�̃{�b�N�X
	AABB m_worldBox;                                                        // ���[���h��Ԃł̃{�b�N�X

	bool m_rotatable;                                                       // ��]�������邩


	friend class PhysicsWorld;                                              // �t�����h��

};