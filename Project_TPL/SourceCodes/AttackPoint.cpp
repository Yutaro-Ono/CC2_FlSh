#include "AttackPoint.h"
#include "Collision.h"
#include "BoxColliderComponent.h"

AttackPoint::AttackPoint(OBJECT_TAG _attackTag, const Vector3& _sizeMin, const Vector3& _sizeMax)
	:Actor(_attackTag)
	,m_attackBox(nullptr)
{
	// �����蔻��{�b�N�X�̐���
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
/// �U�������蔻��{�b�N�X�̍ŏ��E�ő�T�C�Y���Z�b�g����
/// </summary>
/// <param name="_sizeMin"> �ŏ��T�C�Y </param>
/// <param name="_sizeMax"> �ő�T�C�Y </param>
void AttackPoint::SetSizeOfAttackBox(const Vector3& _sizeMin, const Vector3& _sizeMax)
{
	// AABB�̍ăZ�b�g
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
