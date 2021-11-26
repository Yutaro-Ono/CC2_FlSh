#include "LineColliderComponent.h"
#include "GameMain.h"
#include "Actor.h"
#include "BoxColliderComponent.h"
#include "WallColliderComponent.h"


LineColliderComponent::LineColliderComponent(Actor* _owner, int _updateOrder)
	:ColliderComponent(_owner, COLLIDER_TYPE::TYPE_LINE, _updateOrder)
{
	GAME_INSTANCE.GetPhysics()->AddColliderComponent(this);
}

LineColliderComponent::~LineColliderComponent()
{
	printf("remove LineCollider : [%5d] owner->(0x%p)\n", GetID(), m_owner);
	GAME_INSTANCE.GetPhysics()->RemoveColliderComponent(this);
}

void LineColliderComponent::OnUpdateWorldTransform()
{
	m_worldLine = m_localLine;
	// todo : ü•ª‚Ì‰ñ“]ˆ—‚ð‘‚­
	//if (mRotatable)
	//{
	//   ü•ª‰ñ“]ˆ—
	//}

	// •½sˆÚ“®
	m_worldLine.m_startPoint += m_owner->GetPosition();
	m_worldLine.m_endPoint += m_owner->GetPosition();
}

bool LineColliderComponent::CollisionDetection(ColliderComponent* _other)
{
	return _other->Check(this);
}

bool LineColliderComponent::Check(BoxColliderComponent* _other)
{
	CollisionInfo info;
	if (Intersect(m_worldLine, _other->GetWorldBox(), info))
	{
		SetCollisionInfo(info);
		return true;
	}
	return false;
}

bool LineColliderComponent::Check(WallColliderComponent* _other)
{
	CollisionInfo info;
	if (Intersect(_other->GetWall(), m_worldLine, info))
	{
		SetCollisionInfo(info);
		return true;
	}
	return false;
}

