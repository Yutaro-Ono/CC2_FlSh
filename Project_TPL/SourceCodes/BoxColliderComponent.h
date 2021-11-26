//-----------------------------------------------------------------------+
// BoxColliderクラス
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

	void OnUpdateWorldTransform() override;                                 // ワールド変換時
	void SetObjectBox(const AABB& _box) { m_objectBox = _box; }         // 当たり判定用境界ボックスをセット
	const AABB& GetWorldBox() const { return m_worldBox; }                  // ワールド空間上での境界ボックスを取得
	void SetArrowRotate(bool _value) { m_rotatable = _value; }          // 回転を許可するか

	bool CollisionDetection(ColliderComponent* _other) override;

protected:

	bool Check(class BoxColliderComponent* _other) override;
	bool Check(class WallColliderComponent* _other) override;
	bool Check(class LineColliderComponent* _other) override;



	AABB m_objectBox;                                                       // オブジェクト空間(変換前)のボックス
	AABB m_worldBox;                                                        // ワールド空間でのボックス

	bool m_rotatable;                                                       // 回転を許可するか


	friend class PhysicsWorld;                                              // フレンド化

};