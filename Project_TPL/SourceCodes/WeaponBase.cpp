#include "WeaponBase.h"
#include "Player.h"
#include "SkeletalMeshComponent.h"
#include "AttachWeaponToBoneComponent.h"

WeaponBase::WeaponBase()
	:Actor(OBJECT_TAG::ACTOR_WEAPON)
	,m_owner(nullptr)
	,m_existsOwner(false)
	,m_ownerPlayer(nullptr)
	,m_attachComp(nullptr)
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
	InitWeaponInfo();
}

WeaponBase::~WeaponBase()
{
}

void WeaponBase::UpdateActor(float _deltaTime)
{
	m_weaponInfo.muzzlePos = m_position;
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

/// <summary>
/// �I�[�i�[�A�N�^�[�̉���
/// </summary>
void WeaponBase::RemoveOwnerActor()
{
	m_owner = nullptr;
	m_existsOwner = false;
	m_socketMat = Matrix4::Identity;
}

/// <summary>
/// ���̕�������L����v���C���[�|�C���^�̃Z�b�g
/// </summary>
/// <param name="_player">���L������v���C���[�N���X�|�C���^</param>
void WeaponBase::SetPlayer(Player* _player)
{
	m_ownerPlayer = _player;
	m_existsOwner = true;
}

/// <summary>
/// �v���C���[�|�C���^�̉���
/// </summary>
void WeaponBase::RemovePlayer()
{
	m_ownerPlayer = nullptr;
	m_existsOwner = false;
	m_socketMat = Matrix4::Identity;
}

void WeaponBase::SetWeaponInfo(WeaponInfo& _weapInfo)
{
	m_weaponInfo = _weapInfo;
}


void WeaponBase::InitWeaponInfo()
{
	m_weaponInfo.muzzlePos = m_position;
	m_weaponInfo.RPM = 600;
	m_weaponInfo.RPS = 100;
	m_weaponInfo.effectiveRange = 5000.0f;
	m_weaponInfo.initialVelocity = 200.0f;
	m_weaponInfo.numOfPellets = 1;
}

void WeaponBase::SetWeaponInfo(const Vector3& _muzzlePos, unsigned int _rpm, unsigned int _rps, float _effectRange, float initVelocity, unsigned int _numofPellets)
{
	m_weaponInfo.muzzlePos = _muzzlePos;
	m_weaponInfo.RPM = _rpm;
	m_weaponInfo.RPS = _rps;
	m_weaponInfo.effectiveRange = _effectRange;
	m_weaponInfo.initialVelocity = initVelocity;
	m_weaponInfo.numOfPellets = _numofPellets;
}

/// <summary>
/// �\�P�b�g�s��̃Z�b�g
/// </summary>
/// <param name="_socketMat">�{�[���̃\�P�b�g�s��</param>
void WeaponBase::SetSocketMat(Matrix4 _socketMat)
{
	m_socketMat = _socketMat;
}
