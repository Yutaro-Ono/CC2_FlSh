//----------------------------------------------------------------------------------+
// @file        WallColliderComponent.h
// @brief       壁面当たり判定コンポーネントクラス
// @note        ColliderComponentクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/17   新規作成
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