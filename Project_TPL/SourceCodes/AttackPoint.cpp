#include "AttackPoint.h"
#include "Collision.h"
#include "BoxColliderComponent.h"

AttackPoint::AttackPoint(OBJECT_TAG _attackTag, const Vector3& _sizeMin, const Vector3& _sizeMax)
	:Actor(_attackTag)
	,m_attackBox(nullptr)
{
	// 当たり判定ボックスの生成
	AABB attackBox = AABB(_sizeMin, _sizeMax);
	m_attackBox = new BoxColliderComponent(this);
	m_attackBox->SetObjectBox(attackBox);
	m_attackBox->SetArrowRotate(true);
	m_attackBox->SetEnableCollision(false);
}

AttackPoint::~AttackPoint()
{
}

void AttackPoint::UpdateActor(float _deltaTime)
{
}

void AttackPoint::OnCollisionEnter(ColliderComponent* _ownCollComp, ColliderComponent* _otherCollComp)
{
}

/// <summary>
/// 攻撃当たり判定ボックスの最小・最大サイズをセットする
/// </summary>
/// <param name="_sizeMin"> 最小サイズ </param>
/// <param name="_sizeMax"> 最大サイズ </param>
void AttackPoint::SetSizeOfAttackBox(const Vector3& _sizeMin, const Vector3& _sizeMax)
{
	// AABBの再セット
	AABB attackBox = AABB(_sizeMin, _sizeMax);
	m_attackBox->SetObjectBox(attackBox);
}

void AttackPoint::EnableCollision()
{
	m_attackBox->SetEnableCollision(true);
}

void AttackPoint::DisableCollision()
{
	m_attackBox->SetEnableCollision(false);
}
