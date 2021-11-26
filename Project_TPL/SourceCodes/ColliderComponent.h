//----------------------------------------------------------------------------------+
// @file        ColliderComponent.h
// @brief       当たり判定コンポーネントクラス
// @note        Componentクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/17   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

// 当たり判定タイプの列挙
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

	// Double-Dispatch パターンを使用して衝突判定組み合わせを行う
	virtual bool CollisionDetection(ColliderComponent* _other) = 0;

	// Double-Dispatch パターン：Collider同士のふるまいを定義
	virtual bool Check(class BoxColliderComponent* _other);
	virtual bool Check(class WallColliderComponent* _other);
	virtual bool Check(class LineColliderComponent* _other);


protected:

	OBJECT_TAG m_colliderTag;              // 当たり判定用タグ(オーナーを参照)
	COLLIDER_TYPE m_colliderType;          // 当たり判定の種類
	CollisionInfo m_collisionInfo;         // 当たり判定情報

	friend class PhysicsWorld;
};