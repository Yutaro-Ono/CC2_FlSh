//----------------------------------------------------------------------------------+
// @file        WallColliderComponent.h
// @brief       �ǖʓ����蔻��R���|�[�l���g�N���X
// @note        ColliderComponent�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/17   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "ColliderComponent.h"

class WallColliderComponent : public ColliderComponent
{

public:

	WallColliderComponent(class Actor* _owner, int _updateOrder = 100);
	~WallColliderComponent();

	void SetWall(const Wall& _wall);
	const Wall& GetWall() const { return m_wall; };

	bool CollisionDetection(ColliderComponent* _other) override;

protected:

	bool Check(BoxColliderComponent* _other) override;


	Wall m_wall;

	friend class PhysicsWorld;

};