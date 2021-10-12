#include "WeaponBase.h"
#include "SkeletalMeshComponent.h"

WeaponBase::WeaponBase()
	:Actor(OBJECT_TAG::ACTOR_WEAPON)
	,m_owner(nullptr)
	,m_existsOwner(false)
	,m_socketMat(Matrix4::Identity)
	,m_socketNum(33)
	,m_attackInterval(300)
	,m_attackStartCount(0)
	,m_attackNowCount(0)
	,m_ammoBullet(30)
	,m_ammoCount(30)
	,m_ammoMax(300)
	,m_isMelee(false)
{
	m_position = Vector3(-105.0f, 52.0f, -5.0f);
}

WeaponBase::~WeaponBase()
{
}

void WeaponBase::UpdateActor(float _deltaTime)
{
	// �I�[�i�[�A�N�^�[�����݂���ꍇ
	// �\�P�b�g�s����X�V
	if (m_existsOwner)
	{
		SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
		//m_scale = m_worldTransform.GetScale();
		//m_position = m_worldTransform.GetTranslation();
		//m_rotation = m_owner->GetRotation();
		m_recomputeWorldTransform = false;
		//ComputeWorldTransform();
		m_worldTransform = Matrix4::CreateTranslation(m_position) * 
			/*Matrix4::CreateTranslation(Vector3(-8.0f, 60.0f, 110.0f))*/Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(90.0f))) * Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-85.0f))) * m_socketMat * m_owner->GetWorldTransform();
	}
}

/// <summary>
/// �I�[�i�[�A�N�^���L���̓��I�Z�b�g
/// �\�P�b�g�s����X�V
/// </summary>
/// <param name="_owner">���L������A�N�^�[�|�C���^</param>
void WeaponBase::SetOwnerActor(Actor* _owner)
{
	m_owner = _owner;
	m_existsOwner = true;

	m_socketMat = m_owner->GetSkelComp()->GetBoneMat(m_socketNum);
}

void WeaponBase::RemoveOwnerActor()
{
}

/// <summary>
/// �\�P�b�g�s��̃Z�b�g
/// </summary>
/// <param name="_socketMat">�{�[���̃\�P�b�g�s��</param>
void WeaponBase::SetSocketMat(Matrix4 _socketMat)
{
	m_socketMat = _socketMat;
}
