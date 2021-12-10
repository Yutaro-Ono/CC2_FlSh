#include "BulletActor.h"
#include "ColliderComponent.h"

BulletActor::BulletActor(const Vector3& _shootDirVec)
	:Actor(OBJECT_TAG::ATTACK_BULLET)
	,m_dirVec(_shootDirVec)
	,m_velocity(100.0f)
{
	
}

BulletActor::~BulletActor()
{
}

void BulletActor::UpdateActor(float _deltaTime)
{
	Vector3 shootVec = m_dirVec * m_velocity * _deltaTime;
	SetPosition(shootVec);

}

void BulletActor::OnCollisionEnter(ColliderComponent* _ownCollComp, ColliderComponent* _otherCollComp)
{
	// タグごとに処理を分岐
	OBJECT_TAG otherTag = _otherCollComp->GetOwnerTag();

	// 敵(ゾンビ)に当たった
	if (otherTag == OBJECT_TAG::ACTOR_ENEMY)
	{
		
	}

	// 地形に当たった
	if (otherTag == OBJECT_TAG::STATIC_OBJECT)
	{

	}

}
