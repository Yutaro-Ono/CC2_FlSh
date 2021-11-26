//-----------------------------------------------------------------------+
// BoxColliderComponentクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "BoxColliderComponent.h"
#include "GameMain.h"
#include "Actor.h"
#include "WallColliderComponent.h"
#include "LineColliderComponent.h"

// コンストラクタ
BoxColliderComponent::BoxColliderComponent(Actor * _owner, int _updateOrder)
	:ColliderComponent(_owner, COLLIDER_TYPE::TYPE_BOX, _updateOrder)
	,m_objectBox(Vector3::Zero, Vector3::Zero)
	,m_worldBox(Vector3::Zero, Vector3::Zero)
	,m_rotatable(true)
{
	GAME_INSTANCE.GetPhysics()->AddColliderComponent(this);      // BoxColliderComponentをPhysicsWorldに登録
}

// デストラクタ
BoxColliderComponent::~BoxColliderComponent()
{
	printf("Remove BoxColliderComponent : [%5d] owner->( 0x%p )\n", GetID(), m_owner);
	GAME_INSTANCE.GetPhysics()->RemoveColliderComponent(this);
}


void BoxColliderComponent::OnUpdateWorldTransform()
{
	// オブジェクト空間のボックスにリセット
	m_worldBox = m_objectBox;

	// スケーリング
	m_worldBox.m_min *= m_owner->GetScale().x;
	m_worldBox.m_max *= m_owner->GetScale().x;

	// 回転
	if (m_rotatable)
	{
		m_worldBox.Rotate(m_owner->GetRotation());
	}

	// 平行移動
	m_worldBox.m_min += m_owner->GetPosition();
	m_worldBox.m_max += m_owner->GetPosition();

}

bool BoxColliderComponent::CollisionDetection(ColliderComponent* _other)
{
	return _other->Check(this);
}

bool BoxColliderComponent::Check(BoxColliderComponent* _other)
{
	AABB otherBox, ownBox;
	otherBox = _other->GetWorldBox();
	ownBox = GetWorldBox();
	bool ret = Intersect(otherBox, ownBox);
	if (ret)
	{
		CollisionInfo info;
		Vector3 fixVec;
		CalcCollisionFixVec(ownBox, otherBox, fixVec);
		info.m_fixVec = fixVec;
		SetCollisionInfo(info);
	}
	return ret;
}

bool BoxColliderComponent::Check(WallColliderComponent* _other)
{
	CollisionInfo info;
	if (Intersect(GetWorldBox(), _other->GetWall(), info))
	{
		SetCollisionInfo(info);
		return true;
	}
	return false;
}

bool BoxColliderComponent::Check(LineColliderComponent* _other)
{
	CollisionInfo info;
	if (Intersect(_other->GetLine(), GetWorldBox(), info))
	{
		SetCollisionInfo(info);
		return true;
	}
	return false;
}
