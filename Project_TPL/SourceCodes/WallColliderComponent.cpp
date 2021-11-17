#include "WallColliderComponent.h"
#include "GameMain.h"

WallColliderComponent::WallColliderComponent(Actor* _owner, int _updateOrder)
	:ColliderComponent(_owner, COLLIDER_TYPE::TYPE_WALL, _updateOrder)
	,m_wall()
{
	GAME_INSTANCE.GetPhysics()->AddBoxCollider(this);
}

WallColliderComponent::~WallColliderComponent()
{
	GAME_INSTANCE.GetPhysics()->RemoveBoxCollider(this);
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

bool WallColliderComponent::Check(BoxCollider* _other)
{
	CollisionInfo info;
	if (Intersect(_other->GetWorldBox(), m_wall, info))
	{
		_other->SetInfo(info);
		return true;
	}

	return false;
}
