#include "WeaponAR4.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Player.h"

// �I�[�i�[�A�N�^�[�̃{�[���ɕ�����X�i�b�v����ꍇ�̃{�[���ԍ�
const unsigned int WeaponAR4::SOCKET_NUM_SPINE = 1;          // �w��
const unsigned int WeaponAR4::SOCKET_NUM_RIGHTHAND = 33;     // �E��
const unsigned int WeaponAR4::SOCKET_NUM_LEFTHAND = 10;      // ����

// �{�[���X�i�b�v���̍��W�����p�x�N�g��
const Vector3 WeaponAR4::ADJUST_POS_BASIC = Vector3(-14.0f, -105.0f, 29.0f);
const Vector3 WeaponAR4::ADJUST_POS_IDLE_WEAPONOUT = Vector3(-115.0f, 23.0f, 29.0f);
const Vector3 WeaponAR4::ADJUST_POS_MOVE_WEAPONOUT = Vector3(-57.0f, -15.0f, -118.0f);

const std::string WeaponAR4::AR4_MESH_PATH = "Data/Meshes/Weapons/AR4/SM_AR4.gpmesh";

WeaponAR4::WeaponAR4()
{
	Initialize();
}

WeaponAR4::WeaponAR4(Actor* _owner)
{
	// �I�[�i�[�A�N�^�[���Z�b�g
	m_existsOwner = true;
	m_owner = _owner;

	Initialize();
}

WeaponAR4::~WeaponAR4()
{
}

void WeaponAR4::Initialize()
{
	// AR4�̃��b�V�������[�h
	Mesh* mesh = RENDERER->GetMesh(AR4_MESH_PATH);
	
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	m_scale = Vector3(0.85f, 0.85f, 0.85f);
}

/// <summary>
/// �\�P�b�g�s��X�i�b�v�����̍X�V
/// </summary>
/// <param name="_deltaTime">�f���^�^�C��</param>
void WeaponAR4::UpdateSocketMat(float _deltaTime)
{
	// �v���C���[�̕���o����Ԃɉ�����
	// �X�i�b�v����\�P�b�g��ύX
	if (m_ownerPlayer->GetToggleWeaponOut())
	{
		// �ҋ@��Ԃƈړ���Ԃł̓X�i�b�v����\�P�b�g��ύX
		// �ړ����
		if (m_ownerPlayer->GetPlayerState() != PLAYER_STATE::STATE_WEAPONOUT_IDLE)
		{
			m_socketNum = 10;

			SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
			m_recomputeWorldTransform = false;
			m_worldTransform = Matrix4::CreateScale(m_scale)
			* Matrix4::CreateTranslation(ADJUST_POS_MOVE_WEAPONOUT)
			* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(15.0f)))
			* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(-180.0f)))
			//* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-80.0f)))
			* m_socketMat * m_owner->GetWorldTransform();
		}
		// �ҋ@���
		else
		{
			m_socketNum = 33;

            SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
            m_recomputeWorldTransform = false;
            m_worldTransform = Matrix4::CreateScale(m_scale)
	        * Matrix4::CreateTranslation(ADJUST_POS_IDLE_WEAPONOUT)
	        * Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(80.0f)))
	        * Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(-20.0f)))
	        * Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-80.0f)))
	        * m_socketMat * m_owner->GetWorldTransform();
		}

	}
	else
	{
		m_socketNum = 1;

		SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
		m_recomputeWorldTransform = false;
		m_worldTransform = Matrix4::CreateScale(m_scale)
			* Matrix4::CreateTranslation(ADJUST_POS_BASIC)
			* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-90.0f)))
			* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(90.0f)))
			* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(20.0f)))
			* m_socketMat * m_owner->GetWorldTransform();
	}


}

