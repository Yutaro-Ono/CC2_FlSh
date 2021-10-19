#include "WeaponAR4.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Player.h"
#include "FirstPersonCameraComponent.h"

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
	:m_fpsCamera(nullptr)
{
	Initialize();
}

WeaponAR4::WeaponAR4(Actor* _owner)
	:m_fpsCamera(nullptr)
{
	// �I�[�i�[�A�N�^�[���Z�b�g
	m_existsOwner = true;
	m_owner = _owner;

	Initialize();
}

WeaponAR4::WeaponAR4(Player* _owner)
	:m_fpsCamera(_owner->GetFPSCamera())
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

	//m_position = ADJUST_POS_MOVE_WEAPONOUT;
	m_position = Vector3(0.0f, 10.0f, 103.0f);
	m_scale = Vector3(0.85f, 0.85f, 0.85f);
}

/// <summary>
/// �\�P�b�g�s��X�i�b�v�����̍X�V
/// </summary>
/// <param name="_deltaTime">�f���^�^�C��</param>
void WeaponAR4::UpdateSocketMat(float _deltaTime)
{
	// �v���C���[��������o���Ă���ꍇ�Əo���Ă��Ȃ��ꍇ
	// �X�i�b�v����\�P�b�g��ύX
	if (m_ownerPlayer->GetToggleWeaponOut())
	{
		// �G�C�����
		if (m_ownerPlayer->GetPlayerState() == PLAYER_STATE::STATE_AIM)
		{
			m_socketNum = 10;
			SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));

			if (m_fpsCamera != nullptr)
			{
				Vector3 forwardVec = m_fpsCamera->GetCameraForwardVec();
				//forwardVec.z = 0.0f;
				RotateToNewForward(forwardVec);
			}

			m_recomputeWorldTransform = false;
			//m_worldTransform = Matrix4::CreateScale(m_scale)
			//	//* Matrix4::CreateTranslation(Vector3(-34.0f, 25.0f, -117.0f))
			//	//* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(60.0f)))
			//	//* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(-180.0f)))
			//	* Matrix4::CreateFromQuaternion(m_rotation)
			//	* m_socketMat
			//	* m_owner->GetWorldTransform();

			float yaw = m_fpsCamera->GetYaw();
			float pitch = m_fpsCamera->GetPitch();
			Vector3 rotatePos;
			rotatePos.x = cosf(pitch) * cosf(yaw);
			rotatePos.y = cosf(pitch) * sinf(yaw);
			rotatePos.z = sinf(pitch);


			float radian = Math::ToRadians(pitch);
			Quaternion rot = m_owner->GetRotation();
			Quaternion inc(Vector3::UnitY, radian);
			Quaternion target = Quaternion::Concatenate(rot, inc);


			m_worldTransform =
				//Matrix4::CreateTranslation(m_position)
				//Matrix4::CreateRotationZ(Math::ToRadians(-90.0f))
				Matrix4::CreateFromQuaternion(target);
				//Matrix4::CreateRotationZ(yaw)
				//* Matrix4::CreateRotationY(pitch)
				//* Matrix4::CreateTranslation(m_owner->GetPosition());
				//* Matrix4::CreateRotationY(rotatePos.y)
				//* Matrix4::CreateRotationZ(Math::ToRadians(rotatePos.z));

			return;
		}


		// �ҋ@��Ԃƈړ���Ԃł̓X�i�b�v����\�P�b�g��ύX
		// �ړ����
		if (m_ownerPlayer->GetPlayerState() >= PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD &&
			m_ownerPlayer->GetPlayerState() <= PLAYER_STATE::STATE_WEAPONOUT_SPRINT)
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

			return;
		}
		// �ҋ@���
		else if(m_ownerPlayer->GetPlayerState() == PLAYER_STATE::STATE_WEAPONOUT_IDLE)
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

			return;
		}

	}

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

