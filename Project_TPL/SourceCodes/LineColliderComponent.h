//----------------------------------------------------------------------------------+
// @file        LineColliderComponent.h
// @brief       ���������蔻��R���|�[�l���g�N���X
// @note        ColliderComponent�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/26   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "ColliderComponent.h"


class LineColliderComponent : public ColliderComponent
{

public:

	LineColliderComponent(class Actor* _owner, int _updateOrder = 100);
	~LineColliderComponent();


	void         SetLine(const Line& line) { m_localLine = line; }
	const Line& GetLine() const { return m_worldLine; }

	void SetArrowRotate(bool value) { m_worldLine.m_rotatable = value; }
	void OnUpdateWorldTransform() override;
	bool CollisionDetection(ColliderComponent* _other) override;

protected:


	bool         Check(class BoxColliderComponent* _other) override;
	bool         Check(class WallColliderComponent* _other) override;



	Line         m_localLine;              // ���[�J���ʒu
	Line         m_worldLine;              // ����


	friend class PhysicsWorld;
};