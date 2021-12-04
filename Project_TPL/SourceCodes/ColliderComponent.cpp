#include "ColliderComponent.h"
#include "Actor.h"

ColliderComponent::ColliderComponent(Actor* _owner, COLLIDER_TYPE _type, int _updateOrder)
	:Component(_owner)
	,m_colliderType(_type)
	,m_enableCollision(true)
{
	m_colliderTag = GetOwner()->GetActorTag();
}


/// <summary>
/// 当たり判定情報のクリア
/// </summary>
void ColliderComponent::ClearCollisionInfo()
{
	m_collisionInfo.m_fixVec = Vector3::Zero;
	m_collisionInfo.m_collisionPoint = Vector3::Zero;
}

// 組み合わせ定義は継承先でoverrideする
// 当たり判定組み合わせを定義しないときは常にfalse返す
bool ColliderComponent::Check(BoxColliderComponent* _other)
{
	return false;
}

bool ColliderComponent::Check(WallColliderComponent* _other)
{
	return false;
}

bool ColliderComponent::Check(LineColliderComponent* _other)
{
	return false;
}
