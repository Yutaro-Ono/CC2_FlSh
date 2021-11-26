#include "WallColliderComponent.h"
#include "GameMain.h"
#include "BoxColliderComponent.h"
#include "LineColliderComponent.h"

WallColliderComponent::WallColliderComponent(Actor* _owner, int _updateOrder)
	:ColliderComponent(_owner, COLLIDER_TYPE::TYPE_WALL, _updateOrder)
	,m_wall()
{
	GAME_INSTANCE.GetPhysics()->AddColliderComponent(this);
}

WallColliderComponent::~WallColliderComponent()
{
	printf("Remove WallCollider : [%5d] owner->(0x%p)\n", GetID(), m_owner);
	GAME_INSTANCE.GetPhysics()->RemoveColliderComponent(this);
}

void WallColliderComponent::SetWall(const Wall& _wall)
{
	// スロープなら当たり判定タイプ変更
	if (_wall.m_isSlope)
	{
		ChangeColliderType(COLLIDER_TYPE::TYPE_SLOPE);
	}
	m_wall = _wall;
}

bool WallColliderComponent::CollisionDetection(ColliderComponent* _other)
{
	return _other->Check(this);
}

bool WallColliderComponent::Check(BoxColliderComponent* _other)
{
	CollisionInfo info;
	if (Intersect(_other->GetWorldBox(), m_wall, info))
	{
		_other->SetCollisionInfo(info);
		return true;
	}

	return false;
}
