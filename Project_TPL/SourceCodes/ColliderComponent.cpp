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
/// �����蔻����̃N���A
/// </summary>
void ColliderComponent::ClearCollisionInfo()
{
	m_collisionInfo.m_fixVec = Vector3::Zero;
	m_collisionInfo.m_collisionPoint = Vector3::Zero;
}

// �g�ݍ��킹��`�͌p�����override����
// �����蔻��g�ݍ��킹���`���Ȃ��Ƃ��͏��false�Ԃ�
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
